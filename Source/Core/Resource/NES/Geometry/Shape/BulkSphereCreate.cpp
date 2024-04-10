#include <Resource/NES/Geometry/Shape/BulkSphereCreate.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace Geometry {
namespace Shape {
namespace Sphere {
namespace BulkCreate {

Route::Route(Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;

  // Setup List Of Params
  RequiredParams_.push_back("SimulationID");
  RequiredParams_.push_back("RadiusList_um");
  RequiredParams_.push_back("CenterXList_um");
  RequiredParams_.push_back("CenterYList_um");
  RequiredParams_.push_back("CenterZList_um");
  RequiredParams_.push_back("NameList");


  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES/Geometry/Shape/Sphere/BulkCreate");
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

    


    int SimulationID = Request->get_query_parameter("SimulationID", -1);


    // Upstream Query
    nlohmann::json UpstreamQuery;
    UpstreamQuery["RadiusList_um"] = nlohmann::json::parse(Request->get_query_parameter("RadiusList_um", "[]"));;
    UpstreamQuery["CenterXList_um"] = nlohmann::json::parse(Request->get_query_parameter("CenterXList_um", "[]"));;
    UpstreamQuery["CenterYList_um"] = nlohmann::json::parse(Request->get_query_parameter("CenterYList_um", "[]"));;
    UpstreamQuery["CenterZList_um"] = nlohmann::json::parse(Request->get_query_parameter("CenterZList_um", "[]"));;
    UpstreamQuery["NameList"] = nlohmann::json::parse(Request->get_query_parameter("NameList", "[]"));
    UpstreamQuery["SimulationID"] = SimulationID;

    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, Server_->IsNESClientHealthy_, "Geometry/Shape/Sphere/BulkCreate", UpstreamQuery.dump(), &UpstreamResponseStr);
    if (!UpstreamStatus) {
      Util::SendCode(_Session.get(), 3);
      return;
    }
    nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);




    // Build Response And Send
    UpstreamResponse["StatusCode"] = 0;

    std::cout<<"Creating Bulk Spheres"<<std::endl;

    Util::SendJSON(_Session.get(), &UpstreamResponse);
}

}; // Close Namespace
}; // Close Namespace
}; // Close Namespace
}; // Close Namespace
}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG