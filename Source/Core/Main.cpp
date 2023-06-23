//=============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//=============================================================//

/*
    Description: This is the main file of the system.
    Additional Notes: None
    Date Created: 2021-11-01
*/


#include <Main.h>

using namespace std;
using namespace restbed;

void get_method_handler( const shared_ptr< Session > session )
{
    const auto& request = session->get_request( );
    
    const string body = "Hello, " + request->get_path_parameter( "name" );
    session->close( OK, body, { { "Content-Length", ::to_string( body.size( ) ) } } );
}

int main(int NumArguments, char** ArguemntValues) {
    auto resource = make_shared< Resource >( );
    resource->set_path( "/test/{name: .*}" );
    resource->set_method_handler( "GET", get_method_handler );
    
    auto settings = make_shared< Settings >( );
    settings->set_port( 8000 );
    settings->set_default_header( "Connection", "close" );
    
    Service service;
    service.publish( resource );
    service.start( settings );
    
    return EXIT_SUCCESS;
}

