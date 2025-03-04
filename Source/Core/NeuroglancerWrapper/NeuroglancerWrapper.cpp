#include <thread>
#include <csignal>

#include <pybind11/eval.h>

#include "PythonVersion.h"

#include <nlohmann/json.hpp>

#include <NeuroglancerWrapper/NeuroglancerWrapper.h>


// define module for use later
PYBIND11_EMBEDDED_MODULE(NeuroglancerService, m) {
    m.doc() = "a = 5";
}



namespace BG {
namespace API {


void signalHandler( int signum ) {
    exit(signum);
}



NeuroglancerWrapper::NeuroglancerWrapper(Config::Config &_Config, BG::Common::Logger::LoggingSystem* _Logger) {
    Logger_ = _Logger;
    Config_ = &_Config;

    _Logger->Log("Initializing Python Bindings For Neuroglancer", 5);


   
    // while (true) {
    //     pybind11::exec("pass\n");

    // }
    ThreadRun_ = true;
    ThisThread_ = std::make_unique<std::thread>(&NeuroglancerWrapper::KeepAliveThread, this);

}

NeuroglancerWrapper::~NeuroglancerWrapper() {
    
    // Exit Thread, Join
    ThreadRun_ = false;
    ThisThread_->join();

}


void NeuroglancerWrapper::KeepAliveThread() {


    // Hook custom signal handler
    Logger_->Log("Hooking Custom SigHandler", 4);
    signal(SIGINT, signalHandler);

    // Initialize
    Logger_->Log("Initializing Python Interpreter", 3);
    Guard_ = std::make_unique<pybind11::scoped_interpreter>();


    // Print Python Version
    try {
        pybind11::module sys = pybind11::module::import("sys");
        std::string version = pybind11::str(sys.attr("version"));
        Logger_->Log("Python Version: " + version, 3);
    } catch (const pybind11::error_already_set& e) {
        Logger_->Log("Error getting Python version: " + std::string(e.what()), 10);
    }


    // Setup Neuroglancer
    Logger_->Log("Starting Neuroglancer Service", 2);
    std::string PyVenvDir = "venv/lib/python" + std::to_string(PYTHON_MAJOR_VERSION) + "." + std::to_string(PYTHON_MINOR_VERSION) + "/site-packages";
    Logger_->Log("Attempting to load python from '" + PyVenvDir + "'", 2);

    // Add the virtual environment's site-packages to the Python path
    pybind11::module sys = pybind11::module::import("sys");
    sys.attr("path").attr("append")(pybind11::str(PyVenvDir));


    pybind11::module::import("neuroglancer");
    std::string NeuroglancerInitializationProgram = "import neuroglancer\n";
    NeuroglancerInitializationProgram += "IP='" + Config_->Host + "'\n";
    NeuroglancerInitializationProgram += "Port=" + std::to_string(Config_->NeuroglancerPort) + "\n";
    NeuroglancerInitializationProgram += "neuroglancer.set_server_bind_address(bind_address=IP, bind_port=Port)\n";

    try {
        pybind11::exec(NeuroglancerInitializationProgram.c_str(), pybind11::globals());
    } catch (pybind11::error_already_set) {

        pybind11::module sys = pybind11::module::import("sys");
        pybind11::print(sys.attr("path"));

        // pybind11::get_error();
        std::cout<<NeuroglancerInitializationProgram<<std::endl;

        Logger_->Log("Error during python initialization of neuroglancer, please see above code for errors.\n", 10);
        exit(99);
    }
    Logger_->Log("Started Neuroglancer Service On Port " + std::to_string(Config_->NeuroglancerPort), 2);



    // Get Neuroglancer version
    try {
        pybind11::module importlib_metadata = pybind11::module::import("importlib.metadata");
        pybind11::object neuroglancer_version = importlib_metadata.attr("version")("neuroglancer");
        std::string neuroglancer_version_str = pybind11::cast<std::string>(neuroglancer_version);
        Logger_->Log("Neuroglancer Version: " + neuroglancer_version_str, 3);
    } catch (const pybind11::error_already_set& e) {
        Logger_->Log("Error getting Neuroglancer version: " + std::string(e.what()), 10);
    }


    // Acquire GIL for Python version check
    {
        // pybind11::gil_scoped_acquire acquire;
        try {
            pybind11::module sys = pybind11::module::import("sys");
            std::string version = pybind11::str(sys.attr("version"));
            Logger_->Log("KeepAliveThread Reporting Python Version: " + version, 2);
        } catch (const pybind11::error_already_set& e) {
            Logger_->Log("Error getting KeepAliveThread Python version: " + std::string(e.what()), 10);
        }
    }

    // Acquire GIL for module import
    pybind11::module Module;
    {
        // pybind11::gil_scoped_acquire acquire;
        Module = pybind11::module::import("NeuroglancerService");
    }


    Logger_->Log("Started Neuroglancer Service Thread", 1);


    // Keep track of queue length so we know when something was added
    int LastQueueIndexProcessed = -1;

    while (ThreadRun_) {
    
        // Check if work has been generated
        WorkOrderLock_.lock();
        {

            // if there's work, process all items
            int CurrentIndex = WorkOrders_.size() - 1;
            if (CurrentIndex > LastQueueIndexProcessed) {

                // enumerate through all queued items, probably just one, but if there's more, do them too
                for (unsigned int i = LastQueueIndexProcessed + 1; i <= CurrentIndex; i++) {


                    // Get the order from the list of orders
                    WorkOrder* Order = &WorkOrders_[i];


                    // build a bit of python that'll generate a neuroglancer URL, and run it
                    std::string ExecutionString = "";
                    ExecutionString += "Viewer = neuroglancer.Viewer()\n";
                    ExecutionString += "with Viewer.txn() as s:\n";
                    ExecutionString += "    s.layers['image'] = neuroglancer.ImageLayer(source=f'precomputed://" + Order->DatasetURI_ + "')\n";
                    if (Order->DatasetSegmentationURI_ != "") {
                        ExecutionString += "    s.layers['segmentation'] = neuroglancer.SegmentationLayer(source=f'precomputed://" + Order->DatasetSegmentationURI_ + "')\n";
                    }
                    ExecutionString += "OutputURL = str(Viewer)\n";

                    Module.attr("OutputURL") = "ToBeCreated";
                    pybind11::dict Locals = Module.attr("__dict__");
                    pybind11::exec(ExecutionString, pybind11::globals(), Locals);


                    // now, get the result out of python and put it into the return object
                    pybind11::object Result = Module.attr("OutputURL");
                    Order->GeneratedURL_ = pybind11::cast<std::string>(Result);

                    Logger_->Log("Generated Neuroglancer URL: " + Order->GeneratedURL_, 3);


                    // mark the work order as complete, move on to the next item to work on
                    Order->IsComplete_ = true;

                }

                // finally update the index of served requests
                LastQueueIndexProcessed = CurrentIndex;

            }
        }
        WorkOrderLock_.unlock();

        // keep the interpreter from dying? idk why we need this, but it doesn't work if it's not here for some reason...
        pybind11::exec("pass\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }

    Logger_->Log("Exiting Neuroglancer Service Thread", 1);

}


std::string NeuroglancerWrapper::GetNeuroglancerURL(std::string _DatasetURI, std::string _DatasetSegURI) {

    Logger_->Log("Generating Neuroglancer URL For Dataset At " + _DatasetURI, 4);
    Logger_->Log("Generating Neuroglancer Segmentation URL For Dataset At " + _DatasetSegURI, 4);


    // Aquire Lock to put item in
    WorkOrderLock_.lock();  

    WorkOrder Order;
    Order.DatasetURI_ = _DatasetURI;
    Order.DatasetSegmentationURI_ = _DatasetSegURI;
    WorkOrders_.push_back(Order);
    int Index = WorkOrders_.size() - 1;

    WorkOrderLock_.unlock();

    
    // Now wait until it's been processed
    while (!WorkOrders_[Index].IsComplete_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    // Once done, return result
    Logger_->Log("Created Neuroglancer URL " + WorkOrders_[Index].GeneratedURL_, 3);
    return WorkOrders_[Index].GeneratedURL_;

}

std::string NeuroglancerWrapper::GetVisualizerLink(std::string _Request) {

    // Read query parameters to build the dataset URL
    nlohmann::json Query = nlohmann::json::parse(_Request);
    std::string DatasetHandle = Query["NeuroglancerDataset"];
    // std::string DatasetSegmentationHandle = Query["NeuroglancerSegmentationDataset"];
    Logger_->Log("Creating Complete Neuroglancer Environment For Dataset " + DatasetHandle, 4);

    std::string DatasetURL = "http";
    // std::string DatasetSegmentationURL = "http";
    if (Config_->UseHTTPS) {
        DatasetURL += "s";
    }
    DatasetURL += "://" + Config_->PublicHostDomain + ":" + std::to_string(Config_->PortNumber) + "/Dataset/" + DatasetHandle;

    // // If segmentation is enabled, we'll process it
    // if (DatasetSegmentationHandle != "") {
    //     if (Config_->UseHTTPS) {
    //         DatasetSegmentationURL += "s";
    //     }
    //     DatasetSegmentationURL += "://" + Config_->PublicHostDomain + ":" + std::to_string(Config_->PortNumber) + "/Dataset/" + DatasetHandle;

    // }


    // Now, create the neuroglancer URL
    std::string NeuroglancerURL = GetNeuroglancerURL(DatasetURL + "/Data", DatasetURL + "/Segmentation");
    nlohmann::json Response;
    Response["StatusCode"] = 0;
    Response["NeuroglancerURL"] = NeuroglancerURL;

    // std::cout<<Response.dump()<<std::endl;

    return Response.dump();
}


}; // Close Namespace API
}; // Close Namespace BG