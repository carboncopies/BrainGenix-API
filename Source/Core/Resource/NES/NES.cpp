#include <memory>


#include <Resource/NES/NES.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {


Route::Route(BG::Common::Logger::LoggingSystem* _Logger, Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;
  Logger_ = _Logger;


  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES");
  RouteResource->set_method_handler("POST", Callback);
  _Service.publish(RouteResource);


}

Route::~Route() {

}

void Route::PostHandler(const std::shared_ptr<restbed::Session> _Session, const restbed::Bytes& _Line) {

    // Get The NES JSON Query String
    const std::shared_ptr<const restbed::Request> Request = _Session->get_request();
    std::string JSONQueryString = restbed::String::to_string(Request.get()->get_body());

    // Logger_->Log("Received NES request: " + JSONQueryString, 0); // For DEBUGGING

    // Make the upstream query to NES
    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, Server_->IsNESClientHealthy_, "NES", JSONQueryString, &UpstreamResponseStr);
    if (!UpstreamStatus) {
        Util::SendCode(_Session.get(), 3);
        return;
    }

    // Logger_->Log("Called NES Upstream Query", 0);

    // Forward the Response And Send
    
    Util::SendStringifiedJSON(_Session.get(), UpstreamResponseStr);

}

void Route::RouteCallback(const std::shared_ptr<restbed::Session> _Session) {
    const std::shared_ptr<const restbed::Request> Request = _Session->get_request();
    Server_->TotalQueries++;

    // Auth Check
    if (!Util::IsAuthorized(Request.get())) {
      Util::SendCode(_Session.get(), 4);
      return;
    }

    // Fetch the body so it's available for getting later on
    int ContentLength = Request->get_header("Content-Length", 0);
    _Session->fetch(ContentLength, [this](const std::shared_ptr<restbed::Session> _Session, const restbed::Bytes& _Line){return PostHandler(_Session, _Line);});


}

}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG