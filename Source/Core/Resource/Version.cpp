#include <Resource/Version.h>


namespace BG {
namespace API {
namespace Resource {

namespace Diagnostic {
namespace Version {

Route::Route(Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;

  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/Diagnostic/Version");
  RouteResource->set_method_handler("GET", Callback);
  _Service.publish(RouteResource);

}

Route::~Route() {

}


void Route::RouteCallback(const std::shared_ptr<restbed::Session> _Session) {
    const std::shared_ptr<const restbed::Request> Request = _Session->get_request();
    Server_->TotalQueries++;

    // Build Response
    nlohmann::json Response;
    Response["StatusCode"] = 0;
    Response["Version"] = VERSION;


    // Return Response String As JSON
    std::string Body = Response.dump();
    _Session->close(restbed::OK, Body,
      {
        {"Content-Length", std::to_string(Body.size())},
        {"Content-Type", "application/json"}
      }
    );
}

}; // Close Namespace
}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG