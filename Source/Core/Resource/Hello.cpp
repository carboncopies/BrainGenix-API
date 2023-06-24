#include <Resource/Hello.h>


namespace BG {
namespace API {
namespace Resource {


Hello::Hello(Server::Server *_Server, restbed::Service &_Service) {

  // Copy Server Ref To Local Attribs
  Server_ = _Server;

  auto resource = std::make_shared<restbed::Resource>();
  resource->set_path("/Hello");


  auto Callback(std::bind(&Hello::RouteCallback, this, std::placeholders::_1));

  resource->set_method_handler("GET", Callback);
  _Service.publish(resource);
}


Hello::~Hello() {

}


void Hello::RouteCallback(const std::shared_ptr<restbed::Session> session) {

    
    const auto& request = session->get_request( );
    
    std::string name;
    std::string default_value="undefined";
    name = request->get_query_parameter("name", default_value );

    // const string body = "Hello, " + request->get_path_parameter( "name" );
    std::string body = "";
    std::cout<<name<<std::endl;
    if (name == "undefined") {
      body = "{StatusCode=400}";
    }else{
      body = "{Statuscode=500}";
    }
    
    session->close( restbed::OK, body, { { "Content-Length", std::to_string( body.size( ) ) } } );
}

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG