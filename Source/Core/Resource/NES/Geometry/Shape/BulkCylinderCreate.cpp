#include <Resource/NES/Geometry/Shape/BulkCylinderCreate.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace Geometry {
namespace Shape {
namespace Cylinder {
namespace BulkCreate {

Route::Route(Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;

  // Setup List Of Params
  RequiredParams_.push_back("SimulationID");
  RequiredParams_.push_back("Point1RadiusList_um");
  RequiredParams_.push_back("Point2RadiusList_um");
  RequiredParams_.push_back("Point1XList_um");
  RequiredParams_.push_back("Point1YList_um");
  RequiredParams_.push_back("Point1ZList_um");
  RequiredParams_.push_back("Point2XList_um");
  RequiredParams_.push_back("Point2YList_um");
  RequiredParams_.push_back("Point2ZList_um");
  RequiredParams_.push_back("NameList");


  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES/Geometry/Shape/Cylinder/BulkCreate");
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
    UpstreamQuery["Point1RadiusList_um"] = nlohmann::json::parse(Request->get_query_parameter("Point1RadiusList_um", "[]"));
    UpstreamQuery["Point2RadiusList_um"] = nlohmann::json::parse(Request->get_query_parameter("Point2RadiusList_um", "[]"));
    UpstreamQuery["Point1XList_um"] = nlohmann::json::parse(Request->get_query_parameter("Point1XList_um", "[]"));
    UpstreamQuery["Point1YList_um"] = nlohmann::json::parse(Request->get_query_parameter("Point1YList_um", "[]"));
    UpstreamQuery["Point1ZList_um"] = nlohmann::json::parse(Request->get_query_parameter("Point1ZList_um", "[]"));
    UpstreamQuery["Point2XList_um"] = nlohmann::json::parse(Request->get_query_parameter("Point2XList_um", "[]"));
    UpstreamQuery["Point2YList_um"] = nlohmann::json::parse(Request->get_query_parameter("Point2YList_um", "[]"));
    UpstreamQuery["Point2ZList_um"] = nlohmann::json::parse(Request->get_query_parameter("Point2ZList_um", "[]"));
    UpstreamQuery["NameList"] = nlohmann::json::parse(Request->get_query_parameter("NameList", "[]"));
    UpstreamQuery["SimulationID"] = SimulationID;

    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, Server_->IsNESClientHealthy_, "Geometry/Shape/Cylinder/BulkCreate", UpstreamQuery.dump(), &UpstreamResponseStr);
    if (!UpstreamStatus) {
      Util::SendCode(_Session.get(), 3);
      return;
    }
    nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);




    // Build Response And Send
    UpstreamResponse["StatusCode"] = 0;

    std::cout<<"Creating Bulk Cylinders"<<std::endl;

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