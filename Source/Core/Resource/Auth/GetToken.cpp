#include <Resource/Auth/GetToken.h>


namespace BG {
namespace API {
namespace Resource {

namespace Auth {
namespace GetToken {

Route::Route(Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;

  // Setup List Of Params
  RequiredParams_.push_back("Username");
  RequiredParams_.push_back("Password");
  
  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/Auth/GetToken");
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

    // We'd do the database stuff here but that's not yet implemented so....
    // -----------------------------------------------------------------------
    // FIXME: ADD DATABASE CODE HERE TO CHECK THIS IN THE DB! ALSO ADD IT TO THE AUTH-HANDLER (add feature flags! (like cloudflare))
    // -----------------------------------------------------------------------
    std::string Username = Request->get_query_parameter("Username", "");
    std::string Password = Request->get_query_parameter("Password", "");
    if (Username != "Admonishing") {
      Util::SendCode(_Session.get(), 4);
      return;
    }
    if (Password != "Instruction") {
      Util::SendCode(_Session.get(), 4);
    }


    // Build Response
    nlohmann::json Response;
    Response["StatusCode"] = 0;
    Response["AuthKey"] = "MyVerySecureToken";


    Util::SendJSON(_Session.get(), &Response);
}

}; // Close Namespace
}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG