#include <Resource/NES/SphereCreate.h>


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
  RequiredParams_.push_back("Radius_nm");
  RequiredParams_.push_back("Center_nm");
  
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
    float Radius_nm = Request->get_query_parameter("Radius_nm", -1);
    nlohmann::json Center_nm = nlohmann::json::parse(Request->get_query_parameter("Center_nm", "[0, 0, 0]"));
    float CenterPosX_nm = Center_nm[0].template get<float>();
    float CenterPosY_nm = Center_nm[1].template get<float>();
    float CenterPosZ_nm = Center_nm[2].template get<float>();
    int SimulationID = Request->get_query_parameter("SimulationID", 0);
    std::string Name = Request->get_query_parameter("Name", "undefined");


    // Upstream Query
    nlohmann::json UpstreamQuery;
    UpstreamQuery["Radius_nm"] = Radius_nm;
    UpstreamQuery["CenterPosX_nm"] = CenterPosX_nm;
    UpstreamQuery["CenterPosY_nm"] = CenterPosY_nm;
    UpstreamQuery["CenterPosZ_nm"] = CenterPosZ_nm;
    UpstreamQuery["Name"] = Name;
    UpstreamQuery["SimulationID"] = SimulationID;

    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, "Geometry/Shape/Sphere/Create", UpstreamQuery.dump(), &UpstreamResponseStr);
    std::cout<<UpstreamResponseStr<<std::endl;
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