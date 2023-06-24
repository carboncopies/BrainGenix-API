#include <Server/Endpoints.h>


namespace BG {
namespace API {
namespace Server {


EndpointManager::EndpointManager() {

}


EndpointManager::~EndpointManager() {
    
}


void EndpointManager::AddRoutes(restbed::Service &_Service, Server &_Server) {

    // Add Routes
    Hello_ = std::make_shared<Resource::Hello::Route>(&_Server, _Service);
    Diagnostic_Version_ = std::make_shared<Resource::Diagnostic::Version::Route>(&_Server, _Service);
    

}


}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG