#include <Resource/NES/Visualizer/RenderImage.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace Visualizer {
namespace RenderImage {


Route::Route(BG::Common::Logger::LoggingSystem* _Logger, Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;
  Logger_ =_Logger;

  // Setup List Of Params
  RequiredParams_.push_back("SimulationID");
  RequiredParams_.push_back("CameraPosition_um");
  RequiredParams_.push_back("CameraLookAtPosition_um");
  RequiredParams_.push_back("CameraFOV_deg");


  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES/Visualizer/RenderImage");
  RouteResource->set_method_handler("GET", Callback);
  _Service.publish(RouteResource);


}

Route::~Route() {

}


void Route::RouteCallback(const std::shared_ptr<restbed::Session> _Session) {
    const std::shared_ptr<const restbed::Request> Request = _Session->get_request();
    Server_->TotalQueries++;

    // Check Parameters, Early Out If Invalid
    if (!Util::HasRequiredParams(Request.get(), &RequiredParams_)) {
      Util::SendCode(_Session.get(), 2);
      return;
    }

    // Auth Check
    if (!Util::IsAuthorized(Request.get())) {
      Util::SendCode(_Session.get(), 4);
      return;
    }

    

    // Get Params, Build Upstream Query
    nlohmann::json UpstreamQuery;
    
    UpstreamQuery["CameraPosition_um"] = nlohmann::json::parse(Request->get_query_parameter("CameraPosition_um", "[]"));;
    UpstreamQuery["CameraLookAtPosition_um"] = nlohmann::json::parse(Request->get_query_parameter("CameraLookAtPosition_um", "[]"));;

    UpstreamQuery["SimulationID"] = Request->get_query_parameter("SimulationID", 0);
    UpstreamQuery["CameraFOV_deg"] = Request->get_query_parameter("CameraFOV_deg", -1.);


    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, Server_->IsNESClientHealthy_, "Visualizer/RenderImage", UpstreamQuery.dump(), &UpstreamResponseStr);
    if (!UpstreamStatus) {
      Util::SendCode(_Session.get(), 3);
      return;
    }
    nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);



    // Build Response And Send
    nlohmann::json Response;
    Response["StatusCode"] = 0;
    Response["ShapeID"] = UpstreamResponse["ShapeID"].template get<int>();

    Logger_->Log("Rendered Image On Simulation " + std::to_string(Request->get_query_parameter("SimulationID", 0)), 1);

    Util::SendJSON(_Session.get(), &Response);
}

}; // Close Namespace
}; // Close Namespace
}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG