#include <Resource/NES/VSDA/Calcium/CreateIndicator.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace VSDA {
namespace Calcium {
namespace CreateIndicator {


Route::Route(BG::Common::Logger::LoggingSystem* _Logger,Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;
  Logger_= _Logger;

  // Setup List Of Params
  RequiredParams_.push_back("SimulationID");
  RequiredParams_.push_back("CalciumIndicatorName");
  RequiredParams_.push_back("IndicatorRiseTime_ms");
  RequiredParams_.push_back("IndicatorDecayTime_ms");
  RequiredParams_.push_back("IndicatorInterval_ms");
  RequiredParams_.push_back("VisibleComponentsList");
  

  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES/VSDA/Calcium/CreateIndicator");
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
    int SimID = Request->get_query_parameter("SimulationID", -1);
    std::string CalciumIndicatorName = Request->get_query_parameter("CalciumIndicatorName", "");
    float IndicatorRiseTime_ms = Request->get_query_parameter("IndicatorRiseTime_ms", -1.);
    float IndicatorDecayTime_ms = Request->get_query_parameter("IndicatorDecayTime_ms", -1.);
    float IndicatorInterval_ms = Request->get_query_parameter("IndicatorInterval_ms", -1.);
    std::string VisibleComponentsListStr = Request->get_query_parameter("VisibleComponentsList", "");
    std::vector<std::string> VisibleComponentsList = nlohmann::json(VisibleComponentsListStr).get<std::vector<std::string>>();


    // Upstream Query
    nlohmann::json UpstreamQuery;
    UpstreamQuery["SimulationID"]              = SimID;
    UpstreamQuery["CalciumIndicatorName"]      = CalciumIndicatorName;
    UpstreamQuery["IndicatorRiseTime_ms"]      = IndicatorRiseTime_ms;
    UpstreamQuery["IndicatorDecayTime_ms"]     = IndicatorDecayTime_ms;
    UpstreamQuery["IndicatorInterval_ms"]      = IndicatorInterval_ms;
    UpstreamQuery["VisibleComponentsList"]     = VisibleComponentsList;


    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, Server_->IsNESClientHealthy_, "VSDA/Calcium/CreateIndicator", UpstreamQuery.dump(), &UpstreamResponseStr);
    if (!UpstreamStatus) {
      Util::SendCode(_Session.get(), 3);
      return;
    }
    nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);


    // Send Response
    UpstreamResponse["StatusCode"] = 0;

    Logger_->Log("VSDA Calcium CreateIndicator Called With Sim ID: "+std::to_string(SimID)+'\n',1);


    Util::SendJSON(_Session.get(), &UpstreamResponse);
}

}; // Close Namespace
}; // Close Namespace
}; // Close Namespace
}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG