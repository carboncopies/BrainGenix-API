#include <Resource/Hello.h>


namespace BG {
namespace API {
namespace Resource {

namespace Hello {

Route::Route(Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;

  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/Hello");
  RouteResource->set_method_handler("GET", Callback);
  _Service.publish(RouteResource);

}

Route::~Route() {

}


void Route::RouteCallback(const std::shared_ptr<restbed::Session> _Session) {
    const std::shared_ptr<const restbed::Request> Request = _Session->get_request();
    Server_->TotalQueries++;

    std::string name;
    std::string default_value="undefined";
    name = Request->get_query_parameter("name", default_value );

    // const string body = "Route, " + Request->get_path_parameter( "name" );
    std::string body = "";
    std::cout<<Server_->TotalQueries<<std::endl;
    if (name == "undefined") {
      body = "{StatusCode=400}";
    }else{
      body = "{Statuscode=500}";
    }
    
    _Session->close( restbed::OK, body, { { "Content-Length", std::to_string( body.size( ) ) } } );
}

}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG