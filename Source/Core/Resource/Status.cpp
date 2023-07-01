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
    int SystemState = 3;
    if (Server_->APIState == BG::SERVICE_HEALTHY){
      OverallState = "Healthy";
      SystemState = 0;
    } else if (Server_->APIState == BG::SERVICE_FAILED) {
      OverallState = "Failed";
      SystemState = 2;
    } else {
      OverallState = "Degraded";
      SystemState = 1;
    }

    // Build JSON Response
    nlohmann::json Response;
    Response["StatusCode"] = 0;
    Response["SystemState"] = OverallState;
    Response["ServiceStateNES"] = (int)Server_->NESState;
    Response["ServiceStateAPI"] = SystemState;
    Response["ServiceStateAPIDB"] = 3; // not yet configured
    Response["ServiceStateERS"] = 3; // not yet configured
    Response["ServiceStateSTS"] = 3; // not yet configured

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