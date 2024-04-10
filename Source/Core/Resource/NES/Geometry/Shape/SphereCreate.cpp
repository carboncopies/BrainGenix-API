#include <Resource/NES/Geometry/Shape/SphereCreate.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace Geometry {
namespace Shape {
namespace Sphere {
namespace Create {

Route::Route(Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;

  // Setup List Of Params
  RequiredParams_.push_back("SimulationID");
  RequiredParams_.push_back("Radius_um");
  RequiredParams_.push_back("Center_um");
  
  OptionalParams_.push_back("Name");


  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES/Geometry/Shape/Sphere/Create");
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

    


    // Get Params
    float Radius_um = Request->get_query_parameter("Radius_um", -1.0);
    std::cout << "Sphere Radius_um: " << Radius_um << '\n';
    nlohmann::json Center_um = nlohmann::json::parse(Request->get_query_parameter("Center_um", "[0, 0, 0]"));
    float CenterPosX_um = Center_um[0].template get<float>();
    float CenterPosY_um = Center_um[1].template get<float>();
    float CenterPosZ_um = Center_um[2].template get<float>();
    int SimulationID = Request->get_query_parameter("SimulationID", -1);
    std::string Name = Request->get_query_parameter("Name", "undefined");


    // Upstream Query
    nlohmann::json UpstreamQuery;
    UpstreamQuery["Radius_um"] = Radius_um;
    UpstreamQuery["CenterPosX_um"] = CenterPosX_um;
    UpstreamQuery["CenterPosY_um"] = CenterPosY_um;
    UpstreamQuery["CenterPosZ_um"] = CenterPosZ_um;
    UpstreamQuery["Name"] = Name;
    UpstreamQuery["SimulationID"] = SimulationID;

    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, Server_->IsNESClientHealthy_, "Geometry/Shape/Sphere/Create", UpstreamQuery.dump(), &UpstreamResponseStr);
    if (!UpstreamStatus) {
      Util::SendCode(_Session.get(), 3);
      return;
    }
    nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);




    // Build Response And Send
    nlohmann::json Response;
    Response["StatusCode"] = 0;
    Response["ShapeID"] = UpstreamResponse["ShapeID"].template get<int>();

    std::cout<<"Creating Sphere with ID "<<Response["ShapeID"]<<std::endl;

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