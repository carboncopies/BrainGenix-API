#include <Resource/Version.h>


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
    if (Server_->APIState == Server::API_HEALTHY){
      OverallState = "Healthy";
    } else if (Server->APIState == Server::API_DEGRADED) {
      OverallState = "Degraded";
    } else if (Server->APIState == Server::API_FAILED) {
      OverallState = "Failed";
    }

    // Build JSON Response
    rapidjson::Document ResponseJSON;
    ResponseJSON.SetObject();
    ResponseJSON.AddMember("SystemState", OverallState, ResponseJSON.GetAllocator());
    ResponseJSON.AddMember("ServiceStateNES", Server_->NESState, ResponseJSON.GetAllocator());


    // Stringify JSON
    rapidjson::StringBuffer StrBuf;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> Writer(StrBuf);
    ResponseJSON.Accept(Writer);
    std::string Body = StrBuf.GetString();
    
    
    // Return 'Body' String As JSON
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