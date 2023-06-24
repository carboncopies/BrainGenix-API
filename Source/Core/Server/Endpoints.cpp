#include <Resource/Endpoints.h>


namespace BG {
namespace API {
namespace Resource {


void AddRoutes(restbed::Service &_Service, Server::Server &_Server) {

    // Add Hello Route
    Route::Hello Hello(&_Server, _Service);
    

}


}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG