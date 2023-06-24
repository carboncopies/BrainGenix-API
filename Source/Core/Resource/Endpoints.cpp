#include <Resource/Endpoints.h>


namespace BG {
namespace API {
namespace Resource {


void AddRoutes(restbed::Service &_Service, Server::Server &_Server) {

    // Add Hello Route
    auto resource = std::make_shared<restbed::Resource >();
    resource->set_path("/test");
    auto Callback(std::bind(Route::Hello, _Server, std::placeholders::_1));
    resource->set_method_handler( "GET", Callback);
    _Service.publish(resource);

}


}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG