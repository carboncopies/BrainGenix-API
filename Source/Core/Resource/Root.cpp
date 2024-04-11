#include <Resource/Root.h>


namespace BG {
namespace API {
namespace Resource {

namespace Root {

Route::Route(Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;

  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/");
  RouteResource->set_method_handler("GET", Callback);
  _Service.publish(RouteResource);

}

Route::~Route() {

}

void Route::RouteCallback(const std::shared_ptr<restbed::Session> _Session) {
    Server_->TotalQueries++;
    std::string Body = "Welcome to the BrainGenix API. For usage, please see our documentation at https://gitlab.braingenix.org/carboncopies/BrainGenix-API";
    _Session->close( restbed::OK, Body, { { "Content-Length", std::to_string( Body.size( ) ) } } );
}

}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG