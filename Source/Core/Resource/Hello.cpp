#include <Resource/Hello.h>


namespace BG {
namespace API {
namespace Resource {

namespace Hello {

Route::Route(Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;

  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/Hello");
  RouteResource->set_method_handler("GET", Callback);
  _Service.publish(RouteResource);

}

Route::~Route() {

}


void Route::RouteCallback(const std::shared_ptr<restbed::Session> _Session) {
    Server_->TotalQueries++;

    std::string Body = "https://www.youtube.com/watch?v=dQw4w9WgXcQ\nGet Rickrolled lol.";
    _Session->close( restbed::OK, Body, { { "Content-Length", std::to_string( Body.size( ) ) } } );
}

}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG