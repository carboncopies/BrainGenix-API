#include <Resource/Endpoints.h>


namespace BG {
namespace API {
namespace Server {


EndpointManager::EndpointManager() {

}


EndpointManager::~EndpointManager() {
    
}


void EndpointManager::AddRoutes(restbed::Service &_Service, Server::Server &_Server) {

    // Add Hello Route
    Resource::Hello Hello(&_Server, _Service);
    

}


}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG