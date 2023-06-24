#include <Resource/Forbidden.h>


namespace BG {
namespace API {
namespace Resource {

namespace Forbidden {

Route::Route(Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;

  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  _Service.set_method_not_allowed_handler(Callback);

}

Route::~Route() {

}


void Route::RouteCallback(const std::shared_ptr<restbed::Session> _Session) {

    // Return Response String As JSON
    std::string Body = "403 Forbidden";
    _Session->close(restbed::OK, Body,
      {
        {"Content-Length", std::to_string(Body.size())}
      }
    );
}

}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG