#include <thread>

#include <NeuroglancerWrapper/NeuroglancerWrapper.h>




namespace BG {
namespace API {






NeuroglancerWrapper::NeuroglancerWrapper(Config::Config &_Config, BG::Common::Logger::LoggingSystem* _Logger) {


//     // Initialize
//     Py_Initialize();
//     pybind11::pybind11_init();
// `
//     StrategyServer server;

//     py::object main     = py::module::import("__main__");
//     py::object globals  = main.attr("__dict__");
//     py::object module   = import("strategy", "strategy.py", globals);
//     py::object Strategy = module.attr("Strategy");
//     py::object strategy = Strategy(&server); // have to pass server as a pointer now

//     strategy.attr("eval")();

}

NeuroglancerWrapper::~NeuroglancerWrapper() {
    
}




}; // Close Namespace API
}; // Close Namespace BG