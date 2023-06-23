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
    stringstream id;
    id << ::this_thread::get_id( );
    auto body = String::format( "Hello From Thread %s\n", id.str( ).data( ) );
    
    session->close( OK, body, { { "Content-Length", ::to_string( body.length( ) ) } } );
}
int main(int NumArguments, char** ArguemntValues) {
    auto resource = make_shared< Resource >( );
    resource->set_path( "/resource" );
    resource->set_method_handler( "GET", get_method_handler );
    
    auto settings = make_shared< Settings >( );
    settings->set_port( 8000 );
    settings->set_worker_limit( 4 );
    settings->set_default_header( "Connection", "close" );
    
    Service service;
    service.publish( resource );
    service.start( settings );
    
    return EXIT_SUCCESS;
}

