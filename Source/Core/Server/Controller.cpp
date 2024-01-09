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
    EndpointManager_ = EndpointManager();
    EndpointManager_.AddRoutes(Service_, Server_);

}

Controller::~Controller() {

}



std::shared_ptr<restbed::Settings> Controller::ConfigureServer(Config::Config &_Config) {

    // If We Have SSL Enabled, Use That

    std::shared_ptr<restbed::SSLSettings> SSLSettings;

    if (_Config.UseHTTPS) { 

        // Generate URI Based On File Paths
        std::string PrivateKeyURI = std::string("file://") + _Config.KeyFilePath;
        std::string CertificateURI = std::string("file://") + _Config.CrtFilePath;
        std::string DiffiehellmanURI = std::string("file://") + _Config.PemFilePath;
        
        // Setup SSL Settings
        SSLSettings = std::make_shared<restbed::SSLSettings>();
        SSLSettings->set_http_disabled(true);
        SSLSettings->set_private_key(restbed::Uri(PrivateKeyURI));
        SSLSettings->set_certificate(restbed::Uri(CertificateURI));
        SSLSettings->set_temporary_diffie_hellman(restbed::Uri(DiffiehellmanURI));
    }

    // Create Settings Shared Pointer
    std::shared_ptr<restbed::Settings> Settings;
    if (_Config.UseHTTPS) {
        Settings = std::make_shared<restbed::Settings>(SSLSettings);
    } else {
        Settings = std::make_shared<restbed::Settings>();
    }

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

Server* Controller::GetServerStruct() {
    return &Server_;
}


}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG