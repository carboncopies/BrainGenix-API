#include <Resource/Status.h>


namespace BG {
namespace API {
namespace Resource {

namespace Diagnostic {
namespace Status {

Route::Route(Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;

  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/Diagnostic/Status");
  RouteResource->set_method_handler("GET", Callback);
  _Service.publish(RouteResource);

}

Route::~Route() {

}


void Route::RouteCallback(const std::shared_ptr<restbed::Session> _Session) {
    const std::shared_ptr<const restbed::Request> Request = _Session->get_request();
    Server_->TotalQueries++;

    // Setup Response
    std::string OverallState = "";
    if (Server_->APIState == Server::SERVICE_HEALTHY){
      OverallState = "Healthy";
    } else if (Server_->APIState == Server::SERVICE_FAILED) {
      OverallState = "Failed";
    } else {
      OverallState = "Degraded";
    }

    // Build JSON Response
    nlohmann::json Response;
    Response["SystemState"] = OverallState;
    Response["ServiceStateNES"] = (int)Server_->NESState;

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