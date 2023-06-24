#include <Server/Endpoints.h>


namespace BG {
namespace API {
namespace Server {


EndpointManager::EndpointManager() {

}


EndpointManager::~EndpointManager() {
    
}


void EndpointManager::AddRoutes(restbed::Service &_Service, Server &_Server) {

    // Add Defualt Routes (not found/forbidden, etc.)
    NotFound_ = std::make_shared<Resource::NotFound::Route>(&_Server, _Service);
    Forbidden_ = std::make_shared<Resource::Forbidden::Route>(&_Server, _Service);

    // Add Routes
    Hello_ = std::make_shared<Resource::Hello::Route>(&_Server, _Service);
    Diagnostic_Version_ = std::make_shared<Resource::Diagnostic::Version::Route>(&_Server, _Service);
    Diagnostic_Status_ = std::make_shared<Resource::Diagnostic::Status::Route>(&_Server, _Service);
    

}


}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG