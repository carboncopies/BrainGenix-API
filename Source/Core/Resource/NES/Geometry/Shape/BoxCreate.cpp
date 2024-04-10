#include <Resource/NES/Geometry/Shape/BoxCreate.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace Geometry {
namespace Shape {
namespace Box {
namespace Create {

Route::Route(BG::Common::Logger::LoggingSystem* _Logger, Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;
  Logger_ =_Logger;

  // Setup List Of Params
  RequiredParams_.push_back("SimulationID");
  RequiredParams_.push_back("CenterPosition_um");
  RequiredParams_.push_back("Dimensions_um");
  RequiredParams_.push_back("Rotation_rad");

  OptionalParams_.push_back("Name");


  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES/Geometry/Shape/Box/Create");
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
    
    Util::SetVec3(&UpstreamQuery, Request->get_query_parameter("CenterPosition_um", "[0, 0, 0]"), "CenterPos");
    Util::SetVec3(&UpstreamQuery, Request->get_query_parameter("Dimensions_um", "[0, 0, 0]"), "Scale");
    Util::SetVec3(&UpstreamQuery, Request->get_query_parameter("Rotation_rad", "[0, 0, 0]"), "Rotation", "rad");

    UpstreamQuery["SimulationID"] = Request->get_query_parameter("SimulationID", 0);
    UpstreamQuery["Name"] = Request->get_query_parameter("Name", "undefined");


    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, Server_->IsNESClientHealthy_, "Geometry/Shape/Box/Create", UpstreamQuery.dump(), &UpstreamResponseStr);
    if (!UpstreamStatus) {
      Util::SendCode(_Session.get(), 3);
      return;
    }
    nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);



    // Build Response And Send
    nlohmann::json Response;
    Response["StatusCode"] = 0;
    Response["ShapeID"] = UpstreamResponse["ShapeID"].template get<int>();

    Logger_->Log("Creating Box with ID " + std::to_string(static_cast<int>(Response["ShapeID"])) + '\n', 1);

    Util::SendJSON(_Session.get(), &Response);
}

}; // Close Namespace
}; // Close Namespace
}; // Close Namespace
}; // Close Namespace
}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG