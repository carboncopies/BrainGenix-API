//=============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//=============================================================//

/*
    Description: This is the main file of the system.
    Additional Notes: None
    Date Created: 2021-11-01
*/


#include <Main.h>



void get_method_handler( const std::shared_ptr< restbed::Session > session )
{
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

int main(int NumArguments, char** ArgumentValues) {

    // Startup With Config Manager, Will Read Args And Config File, Then Parse Into Config Struct
    BG::API::Config::Manager ConfigManager(NumArguments, ArgumentValues);
    BG::API::Config::Config& SystemConfiguration = ConfigManager.GetConfig();

    // Setup Controller
    BG::API::Server::Controller ServerController(SystemConfiguration);
    

    auto resource = std::make_shared< restbed::Resource >( );
    resource->set_path( "/test" );
    resource->set_method_handler( "GET", get_method_handler );
    
    auto settings = std::make_shared< restbed::Settings >( );
    settings->set_port( 8000 );
    settings->set_default_header( "Connection", "close" );
    
    restbed::Service service;
    service.publish( resource );
    service.start( settings );
    
    return EXIT_SUCCESS;
}

