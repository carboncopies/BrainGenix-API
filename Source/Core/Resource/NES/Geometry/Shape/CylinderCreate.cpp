#include <Resource/NES/Geometry/Shape/CylinderCreate.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace Geometry {
namespace Shape {
namespace Cylinder {
namespace Create {

Route::Route(BG::Common::Logger::LoggingSystem* _Logger,Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;
  Logger_ =_Logger;


  // Setup List Of Params
  RequiredParams_.push_back("SimulationID");
  RequiredParams_.push_back("Point1Radius_um");
  RequiredParams_.push_back("Point1Position_um");
  RequiredParams_.push_back("Point2Radius_um");
  RequiredParams_.push_back("Point2Position_um");

  OptionalParams_.push_back("Name");


  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES/Geometry/Shape/Cylinder/Create");
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
    
    nlohmann::json Point1Position_um = nlohmann::json::parse(Request->get_query_parameter("Point1Position_um", "[0, 0, 0]"));
    nlohmann::json Point2Position_um = nlohmann::json::parse(Request->get_query_parameter("Point2Position_um", "[0, 0, 0]"));

    UpstreamQuery["SimulationID"] = Request->get_query_parameter("SimulationID", 0);
    UpstreamQuery["Name"] = Request->get_query_parameter("Name", "undefined");
    float Point1Radius_um = Request->get_query_parameter("Point1Radius_um", -1.0);
    float Point2Radius_um = Request->get_query_parameter("Point2Radius_um", -1.0);
    Logger_->Log("Point1Radius_um: "+ std::to_string(Point1Radius_um)+'\n',1);
    Logger_->Log("Point1Radius_um: "+ std::to_string(Point2Radius_um)+'\n',1);
    UpstreamQuery["Point1Radius_um"] = Point1Radius_um;
    UpstreamQuery["Point2Radius_um"] = Point2Radius_um;
    UpstreamQuery["Point1PosX_um"] = Point1Position_um[0].template get<float>();
    UpstreamQuery["Point1PosY_um"] = Point1Position_um[1].template get<float>();
    UpstreamQuery["Point1PosZ_um"] = Point1Position_um[2].template get<float>();
    UpstreamQuery["Point2PosX_um"] = Point2Position_um[0].template get<float>();
    UpstreamQuery["Point2PosY_um"] = Point2Position_um[1].template get<float>();
    UpstreamQuery["Point2PosZ_um"] = Point2Position_um[2].template get<float>();


    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, Server_->IsNESClientHealthy_, "Geometry/Shape/Cylinder/Create", UpstreamQuery.dump(), &UpstreamResponseStr);
    if (!UpstreamStatus) {
      Util::SendCode(_Session.get(), 3);
      return;
    }
    nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);



    // Build Response And Send
    nlohmann::json Response;
    Response["StatusCode"] = 0;
    Response["ShapeID"] = UpstreamResponse["ShapeID"].template get<int>();

    Logger_->Log("Creating Cylinder with ID "+ std::to_string(static_cast<int>((Response["ShapeID"])))+'\n',1);

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