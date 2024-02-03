#include <memory>


#include <Resource/NES/NES.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {


Route::Route(BG::Common::Logger::LoggingSystem* _Logger,Server::Server *_Server, restbed::Service &_Service) {
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
    _Session->fetch(ContentLength, [](const std::shared_ptr<restbed::Session> _Session, const restbed::Bytes& _Line){});

    // Get The NES JSON Query String
    std::string JSONQueryString = restbed::String::to_string(Request.get()->get_body());


    // Make the upstream query to NES
    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, "NES", JSONQueryString, &UpstreamResponseStr);
    if (!UpstreamStatus) {
        Util::SendCode(_Session.get(), 3);
        return;
    }


    // Build Response And Send
    nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);
    UpstreamResponse["StatusCode"] = 0;

    Logger_->Log("Called NES Upstream Query", 1);

    Util::SendJSON(_Session.get(), &UpstreamResponse);



}

}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG