#include <Resource/NES/SimulationCreate.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace Simulation {
namespace Create {

Route::Route( BG::Common::Logger::LoggingSystem* _Logger,Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;
  Logger_ = _Logger;

  // Setup List Of Params
  RequiredParams_.push_back("SimulationName");
  

  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES/Simulation/Create");
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
    std::string Name = Request->get_query_parameter("SimulationName", "");


    // Upstream Query
    nlohmann::json UpstreamQuery;
    UpstreamQuery["Name"] = Name;

    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, "Simulation/Create", UpstreamQuery.dump(), &UpstreamResponseStr);
    if (!UpstreamStatus) {
      Util::SendCode(_Session.get(), 1);
      return;
    }
    //nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);



    // Build Response And Send
    nlohmann::json Response;
    // Response["StatusCode"] = 0;
    // Response["SimulationID"] = UpstreamResponse["SimulationID"].template get<int>();
    
    //Logger_->Log("Creating Simulation with ID "+std::to_string(static_cast<int>(Response["SimulationID"]))+'\n',1);
    //Util::SendJSON(_Session.get(), &Response);

    Logger_->Log("Creating Simulation: "+UpstreamResponseStr+'\n',1);
    Util::SendStringifiedJSON(_Session.get(), UpstreamResponseStr);
}

}; // Close Namespace
}; // Close Namespace
}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG