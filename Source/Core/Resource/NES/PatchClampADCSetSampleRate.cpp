#include <Resource/NES/PatchClampADCSetSampleRate.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace Tool {
namespace PatchClampADC {
namespace SetSampleRate {

Route::Route(BG::Common::Logger::LoggingSystem* _Logger,Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;
  Logger_ = _Logger;

  // Setup List Of Params
  RequiredParams_.push_back("SimulationID");
  RequiredParams_.push_back("TargetADC");
  RequiredParams_.push_back("Timestep_ms");

  OptionalParams_.push_back("Name");


  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES/Tool/PatchClampADC/SetSampleRate");
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
    UpstreamQuery["PatchClampADCID"] = Request->get_query_parameter("TargetADC", -1);
    UpstreamQuery["Timestep_ms"] = Request->get_query_parameter("Timestep_ms", -1);


    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, Server_->IsNESClientHealthy_, "Tool/PatchClampADC/SetSampleRate", UpstreamQuery.dump(), &UpstreamResponseStr);
    if (!UpstreamStatus) {
      Util::SendCode(_Session.get(), 3);
      return;
    }
    //nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);



    // Build Response And Send
    // nlohmann::json Response;
    // Response["StatusCode"] = 0;

    Logger_->Log("Setting PatchClampADC Sample Rate On PatchClampADC With ID "+ std::to_string(static_cast<int>(Request->get_query_parameter("TargetADC", 0)))+'\n',1);
    //Util::SendJSON(_Session.get(), &Response);

    Logger_->Log("Setting PatchClampADC Sample Rate On PatchClampADC: "+ UpstreamResponseStr+'\n',1);
    Util::SendStringifiedJSON(_Session.get(), UpstreamResponseStr);
}

}; // Close Namespace
}; // Close Namespace
}; // Close Namespace
}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG