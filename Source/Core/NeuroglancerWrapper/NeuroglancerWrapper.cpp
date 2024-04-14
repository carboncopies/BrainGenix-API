#include <thread>
#include <csignal>

#include <pybind11/eval.h>

#include <NeuroglancerWrapper/NeuroglancerWrapper.h>




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
    NeuroglancerInitializationProgram += "viewer = neuroglancer.Viewer()\n";
    NeuroglancerInitializationProgram += "with viewer.txn() as s:\n";
    NeuroglancerInitializationProgram += "    s.layers['image'] = neuroglancer.ImageLayer(source=f'precomputed://http://localhost:9000/NeuroglancerDataset')\n";
    NeuroglancerInitializationProgram += "print(viewer)\n";
    
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


    Logger_->Log("Started Neuroglancer Service Thread", 1);

    while (ThreadRun_) {
    
        pybind11::exec("pass\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }

    Logger_->Log("Exiting Neuroglancer Service Thread", 1);

}

void NeuroglancerWrapper::test() {

    // pybind11::exec("while True:\n    pass\n", pybind11::globals());

}



}; // Close Namespace API
}; // Close Namespace BG