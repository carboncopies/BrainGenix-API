#include <Resource/NES/SimulationBuildMesh.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace Simulation {
namespace BuildMesh {

Route::Route(BG::Common::Logger::LoggingSystem* _Logger,Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;
  Logger_ = _Logger;

  // Setup List Of Params
  RequiredParams_.push_back("SimulationID");
  

  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES/Simulation/BuildMesh");
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


    // Upstream Query
    nlohmann::json UpstreamQuery;
    UpstreamQuery["SimulationID"] = SimID;

    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, Server_->IsNESClientHealthy_, "Simulation/BuildMesh", UpstreamQuery.dump(), &UpstreamResponseStr);
    if (!UpstreamStatus) {
      Util::SendCode(_Session.get(), 1);
      return;
    }
    //nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);



    // Build Response And Send
    // nlohmann::json Response;
    // Response["StatusCode"] = 0;
    
    //Logger_->Log("Building Simulation Mesh For Sim With ID "+ std::to_string(SimID)+'\n',1);

    //Util::SendJSON(_Session.get(), &Response);

    Logger_->Log("Building Simulation Mesh For Sim: "+ UpstreamResponseStr+'\n',1);
    Util::SendStringifiedJSON(_Session.get(), UpstreamResponseStr);
}

}; // Close Namespace
}; // Close Namespace
}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG