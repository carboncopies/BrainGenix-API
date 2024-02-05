#include <Resource/NES/ReceptorCreate.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace Connection {
namespace Receptor {
namespace Create {

Route::Route(BG::Common::Logger::LoggingSystem* _Logger,Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;
  Logger_ = _Logger;

  // Setup List Of Params
  RequiredParams_.push_back("SimulationID");
  RequiredParams_.push_back("SourceCompartmentID");
  RequiredParams_.push_back("DestinationCompartmentID");
  RequiredParams_.push_back("Conductance_nS");
  RequiredParams_.push_back("TimeConstantRise_ms");
  RequiredParams_.push_back("TimeConstantDecay_ms");
  RequiredParams_.push_back("ReceptorLocation_um");

  OptionalParams_.push_back("Name");


  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES/Connection/Receptor/Create");
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
    UpstreamQuery["DestinationCompartmentID"] = Request->get_query_parameter("DestinationCompartmentID", -1);
    UpstreamQuery["Conductance_nS"] = Request->get_query_parameter("Conductance_nS", 0.0f);
    UpstreamQuery["TimeConstantRise_ms"] = Request->get_query_parameter("TimeConstantRise_ms", 0.0f);
    UpstreamQuery["TimeConstantDecay_ms"] = Request->get_query_parameter("TimeConstantDecay_ms", 0.0f);
    Util::SetVec3(&UpstreamQuery, Request->get_query_parameter("ReceptorLocation_um", "[0, 0, 0]"), "ReceptorPos");



    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, "Connection/Receptor/Create", UpstreamQuery.dump(), &UpstreamResponseStr);
    if (!UpstreamStatus) {
      Util::SendCode(_Session.get(), 3);
      return;
    }
    //nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);



    // Build Response And Send
    // nlohmann::json Response;
    // Response["StatusCode"] = 0;
    // Response["ReceptorID"] = UpstreamResponse["ReceptorID"].template get<int>();

    //Logger_->Log("Creating Receptor Connection with ID "+std::to_string(static_cast<int>(Response["ReceptorID"]))+'\n',1);

    //Util::SendJSON(_Session.get(), &Response);

    Logger_->Log("Creating Receptor Connection: "+UpstreamResponseStr+'\n',1);
    Util::SendStringifiedJSON(_Session.get(), UpstreamResponseStr);
}

}; // Close Namespace
}; // Close Namespace
}; // Close Namespace
}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG