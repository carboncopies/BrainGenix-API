#include <Server/Controller.h>




namespace BG {
namespace API {
namespace Server {



void service_error_handler( const int, const std::exception&, const std::shared_ptr< restbed::Session > session ) {
    if ( session->is_open( ) )
        session->close( 5000, "Custom Service Internal Server Error", { { "Content-Length", "36" } } );
    else
        fprintf( stderr, "Custom Service Internal Server Error\n" );
}


void TextServerHandler(const std::shared_ptr<restbed::Session> _Session) {
    const std::shared_ptr<const restbed::Request> Request = _Session->get_request();
    std::string Filename = "/" + Request->get_path_parameter("filename");
    
    // Strip Potentially Dangerous '..'
    // Filename.erase(std::remove(Filename.begin(), Filename.end(), ".."), Filename.end());
    // boost::erase_all(Filename, "..");

    std::string FinalFilename = "/.well-known/acme-challenge" + Filename;
    std::cout<<"[INFO] User Requested File From "<<FinalFilename<<std::endl;
    std::ifstream Filestream(FinalFilename, std::ifstream::in);
    
    if (Filestream.is_open()) {
        std::string Body = std::string(std::istreambuf_iterator<char>(Filestream), std::istreambuf_iterator<char>());
        
        const std::multimap<std::string, std::string> Headers {
            {"Content-Type", "text/plain"},
            {"Content-Length", std::to_string(Body.length())}
        };
        
        _Session->close(restbed::OK, Body, Headers);
    } else {
        _Session->close(restbed::NOT_FOUND);
    }
}


Controller::Controller(Config::Config &_Config) {
    
    // Configure Settings Object
    Settings_ = ConfigureServer(_Config);


    // Add Routes
    EndpointManager_ = EndpointManager();
    EndpointManager_.AddRoutes(Service_, Server_);

}

Controller::~Controller() {

}



std::shared_ptr<restbed::Settings> Controller::ConfigureServer(Config::Config& _Config) {

    // If We Have SSL Enabled, Use That
    std::shared_ptr<restbed::SSLSettings> SSLSettings;
    if (_Config.UseHTTPS) { 

        std::cout<<"Starting In HTTPS Mode, An Additional HTTP Service Will Be Run On Port 80 For Certbot Verification\n";

        // Generate URI Based On File Paths
        std::string PrivateKeyURI = std::string("file://") + _Config.KeyFilePath;
        std::string CertificateURI = std::string("file://") + _Config.CrtFilePath;
        // std::string DiffiehellmanURI = std::string("file://") + _Config.PemFilePath;
        
        // Setup SSL Settings
        SSLSettings = std::make_shared<restbed::SSLSettings>();
        SSLSettings->set_http_disabled(true);
        SSLSettings->set_certificate_chain(restbed::Uri(CertificateURI));
        SSLSettings->set_private_key(restbed::Uri(PrivateKeyURI));
        SSLSettings->set_port(_Config.PortNumber);


    }

    // Create Settings Shared Pointer
    std::shared_ptr<restbed::Settings> Settings = std::make_shared<restbed::Settings>();
    if (_Config.UseHTTPS) {
        Settings->set_ssl_settings(SSLSettings);
    }

    // Configure Settings Object
    if (_Config.UseHTTPS) {
        Settings->set_port(80);
    } else {
        Settings->set_port(_Config.PortNumber);

    }
    // Settings->set_bind_address(_Config.Host);
    Settings->set_default_header("Connection", "close");

    // Return Configured Settings Object
    return Settings;
}


void Controller::StartService() {

    // Also Expose "/.well-known/acme-challenge" for Let's Encrypt to verify from (oh http service)
    std::shared_ptr<restbed::Resource> Resource = std::make_shared<restbed::Resource>();
    Resource->set_path("/.well-known/acme-challenge/{filename: .*}"); // THIS IS BAD, WE DONT STRIP THINGS, CAUSE IM LAZY!!! FIXME!-This still might be bad - we do strip out '..' but still could be bad.
    Resource->set_method_handler( "GET", TextServerHandler);
    Service_.publish(Resource);


    std::cout<<"Started Main Service\n";
    Service_.start(Settings_);
    Service_.set_error_handler( service_error_handler );

}

Server* Controller::GetServerStruct() {
    return &Server_;
}


}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG