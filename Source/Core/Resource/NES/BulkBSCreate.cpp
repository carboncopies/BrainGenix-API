#include <Resource/NES/BulkBSCreate.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace Compartment {
namespace BS {
namespace BulkCreate {

Route::Route(BG::Common::Logger::LoggingSystem* _Logger, Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;
  Logger_ = _Logger;

  // Setup List Of Params
  RequiredParams_.push_back("SimulationID");
  RequiredParams_.push_back("ShapeIDList");
  RequiredParams_.push_back("MembranePotentialList_mV");
  RequiredParams_.push_back("SpikeThresholdList_mV");
  RequiredParams_.push_back("DecayTimeList_ms");
  RequiredParams_.push_back("RestingPotentialList_mV");
  RequiredParams_.push_back("AfterHyperpolarizationAmplitudeList_mV");
  RequiredParams_.push_back("NameList");


  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES/Compartment/BS/BulkCreate");
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
    UpstreamQuery["NameList"] = nlohmann::json::parse(Request->get_query_parameter("NameList", "[]"));
    UpstreamQuery["AfterHyperpolarizationAmplitudeList_mV"] = nlohmann::json::parse(Request->get_query_parameter("AfterHyperpolarizationAmplitudeList_mV", "[]"));
    UpstreamQuery["RestingPotentialList_mV"] = nlohmann::json::parse(Request->get_query_parameter("RestingPotentialList_mV", "[]"));
    UpstreamQuery["DecayTimeList_ms"] = nlohmann::json::parse(Request->get_query_parameter("DecayTimeList_ms", "[]"));
    UpstreamQuery["SpikeThresholdList_mV"] = nlohmann::json::parse(Request->get_query_parameter("SpikeThresholdList_mV", "[]"));
    UpstreamQuery["MembranePotentialList_mV"] = nlohmann::json::parse(Request->get_query_parameter("MembranePotentialList_mV", "[]"));
    UpstreamQuery["ShapeIDList"] = nlohmann::json::parse(Request->get_query_parameter("ShapeIDList", "[]"));


    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, "Compartment/BS/BulkCreate", UpstreamQuery.dump(), &UpstreamResponseStr);
    if (!UpstreamStatus) {
      Util::SendCode(_Session.get(), 3);
      return;
    }
    nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);



    // Build Response And Send
    nlohmann::json Response;
    UpstreamResponse["StatusCode"] = 0;

    Logger_->Log("Creating BallStick Compartments with IDs "+ std::to_string(static_cast<int>(UpstreamResponse["CompartmentIDs"]))+'\n',1);

    Util::SendJSON(_Session.get(), &UpstreamResponse);
}

}; // Close Namespace
}; // Close Namespace
}; // Close Namespace
}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG