#include <Resource/NES/PatchClampADCCreate.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace Tool {
namespace PatchClampADC {
namespace Create {

Route::Route(Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;

  // Setup List Of Params
  RequiredParams_.push_back("SimulationID");
  RequiredParams_.push_back("SourceCompartmentID");
  RequiredParams_.push_back("ClampLocation_um");

  OptionalParams_.push_back("Name");


  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES/Tool/PatchClampADC/Create");
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
    UpstreamQuery["SimulationID"] = Request->get_query_parameter("SimulationID", -1);
    UpstreamQuery["Name"] = Request->get_query_parameter("Name", "undefined");
    UpstreamQuery["SourceCompartmentID"] = Request->get_query_parameter("SourceCompartmentID", -1);

    Util::SetVec3(&UpstreamQuery, Request->get_query_parameter("ClampLocation_um", "[0, 0, 0]"), "ClampPos");



    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, "Tool/PatchClampADC/Create", UpstreamQuery.dump(), &UpstreamResponseStr);
    if (!UpstreamStatus) {
      Util::SendCode(_Session.get(), 3);
      return;
    }
    nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);



    // Build Response And Send
    nlohmann::json Response;
    Response["StatusCode"] = 0;
    Response["PatchClampADCID"] = UpstreamResponse["PatchClampADCID"].template get<int>();

    std::cout<<"Creating PatchClampADC with ID "<<Response["PatchClampADCID"]<<std::endl;

    Util::SendJSON(_Session.get(), &Response);
}

}; // Close Namespace
}; // Close Namespace
}; // Close Namespace
}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG