#include <thread>
#include <csignal>

#include <pybind11/eval.h>

#include <NeuroglancerWrapper/NeuroglancerWrapper.h>


// define module for use later
PYBIND11_EMBEDDED_MODULE(NeuroglancerService, m) {
    m.doc() = "";
}



namespace BG {
namespace API {


void signalHandler( int signum ) {
    exit(signum);
}



NeuroglancerWrapper::NeuroglancerWrapper(Config::Config &_Config, BG::Common::Logger::LoggingSystem* _Logger) {
    Logger_ = _Logger;

    _Logger->Log("Initializing Python Bindings For Neuroglancer", 5);


    // Hook custom signal handler
    _Logger->Log("Hooking Custom SigHandler", 4);
    signal(SIGINT, signalHandler);


    // Initialize
    _Logger->Log("Initializing Python Interpreter", 3);
    Guard_ = std::make_unique<pybind11::scoped_interpreter>();


    // Setup Neuroglancer
    _Logger->Log("Starting Neuroglancer Service", 2);

    std::string NeuroglancerInitializationProgram = "import neuroglancer\n";
    NeuroglancerInitializationProgram += "IP='" + _Config.Host + "'\n";
    NeuroglancerInitializationProgram += "Port=" + std::to_string(_Config.NeuroglancerPort) + "\n";
    NeuroglancerInitializationProgram += "neuroglancer.set_server_bind_address(bind_address=IP, bind_port=Port)\n";

    try {
        pybind11::exec(NeuroglancerInitializationProgram.c_str(), pybind11::globals());
    } catch (pybind11::error_already_set) {

        pybind11::module sys = pybind11::module::import("sys");
	    pybind11::print(sys.attr("path"));

        // pybind11::get_error();
        std::cout<<NeuroglancerInitializationProgram<<std::endl;

        _Logger->Log("Error during python initialization of neuroglancer, please see above code for errors.\n", 10);
        exit(99);
    }
    _Logger->Log("Started Neuroglancer Service On Port " + std::to_string(_Config.NeuroglancerPort), 2);



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

    // Setup System
    pybind11::module Module = pybind11::module::import("NeuroglancerService");


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
                    ExecutionString += "OutputURL = str(Viewer)\n";

                    Module.attr("OutputURL") = "ToBeCreated";
                    pybind11::dict Locals = Module.attr("__dict__");
                    pybind11::exec(ExecutionString, pybind11::globals(), Locals);


                    // now, get the result out of python and put it into the return object
                    pybind11::object Result = Module.attr("OutputURL");
                    Order->GeneratedURL_ = pybind11::cast<std::string>(Result);


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


std::string NeuroglancerWrapper::GetNeuroglancerURL(std::string _DatasetURI) {

    Logger_->Log("Generating Neuroglancer URL For Dataset At " + _DatasetURI, 4);


    // Aquire Lock to put item in
    WorkOrderLock_.lock();  

    WorkOrder Order;
    Order.DatasetURI_ = _DatasetURI;
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



}; // Close Namespace API
}; // Close Namespace BG