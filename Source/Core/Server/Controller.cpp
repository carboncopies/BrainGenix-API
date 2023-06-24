#include <Server/Controller.h>




namespace BG {
namespace API {
namespace Server {


// void Controller::TestHandler(const std::shared_ptr<restbed::Session> session ) {
//     const auto& request = session->get_request( );
    
//     std::cout<<foo_<<std::endl; //callback testing

//     std::string name;
//     std::string default_value="undefined";
//     name = request->get_query_parameter("name", default_value );

//     // const string body = "Hello, " + request->get_path_parameter( "name" );
//     std::string body = "";
//     std::cout<<name<<std::endl;
//     if (name == "undefined") {
//       body = "{StatusCode=400}";
//     }else{
//       body = "{Statuscode=500}";
//     }
    
//     session->close( restbed::OK, body, { { "Content-Length", std::to_string( body.size( ) ) } } );
// }


Controller::Controller(Config::Config &_Config) {

    // // Create Routes
    // auto resource = std::make_shared< restbed::Resource >( );
    // resource->set_path( "/test" );


    // auto Callback(std::bind(&Controller::TestHandler, this, std::placeholders::_1));

    // resource->set_method_handler( "GET", Callback);
    // Service_.publish(resource);
    
    // Configure Settings Object
    Settings_ = ConfigureServer(_Config);

    // Add Routes
    Resource::AddRoutes(Service_, Server_);

}

Controller::~Controller() {

}



std::shared_ptr<restbed::Settings> Controller::ConfigureServer(Config::Config &_Config) {

    // Create Settings Shared Pointer
    std::shared_ptr<restbed::Settings> Settings = std::make_shared<restbed::Settings>();

    // Configure Settings Object
    Settings->set_port(_Config.PortNumber);
    // Settings->set_bind_address(_Config.Host);
    Settings->set_default_header( "Connection", "close" );

    // Return Configured Settings Object
    return Settings;
}


void Controller::StartService() {
    Service_.start(Settings_);
}


}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG