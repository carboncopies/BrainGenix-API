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

    _Logger->Log("Initializing Python Bindings For Neuroglancer", 5);


    // Hook custom signal handler
    _Logger->Log("Hooking Custom SigHandler", 4);
    signal(SIGINT, signalHandler);


    // Initialize
    _Logger->Log("Initializing Python Interpreter", 3);
    Guard_ = std::make_unique<pybind11::scoped_interpreter>();


    // Setup Neuroglancer
    pybind11::module::import("sys");
    pybind11::exec("sys.path.append('/home/lucas/.local/lib/python3.10/site-packages')");
    // auto mod = pybind11::module::import("random");
    std::string NeuroglancerInitializationProgram = "from math import sqrt\n";
    NeuroglancerInitializationProgram += "IP='" + _Config.Host + "'\n";
    NeuroglancerInitializationProgram += "Port=" + std::to_string(_Config.NeuroglancerPort) + "\n";
    // NeuroglancerInitializationProgram += "neuroglancer.set_server_bind_address(bind_address=IP, bind_port=Port)\n";
    pybind11::exec("print(sys.version)");
    _Logger->Log("Starting Neuroglancer Service", 2);
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
    _Logger->Log("Started Neuroglancer Service", 2);


}

NeuroglancerWrapper::~NeuroglancerWrapper() {
    
}




}; // Close Namespace API
}; // Close Namespace BG