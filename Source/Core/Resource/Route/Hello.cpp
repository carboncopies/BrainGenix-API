#include <Resource/Route/Hello.h>


namespace BG {
namespace API {
namespace Resource {
namespace Route {


void Hello(Server::Server &_Server, const std::shared_ptr<restbed::Session> session) {

    
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

}; // Close Namespace Route
}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG