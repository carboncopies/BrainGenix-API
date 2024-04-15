#include <thread>

#include <Server/Controller.h>




namespace BG {
namespace API {
namespace Server {



void TextServerHandler(const std::shared_ptr<restbed::Session> _Session) {
    const std::shared_ptr<const restbed::Request> Request = _Session->get_request();
    std::string Filename = "/" + Request->get_path_parameter("filename");
    
    // Strip Potentially Dangerous '..'
    std::string Pattern = "..";
    std::string::size_type i = Filename.find(Pattern);
    while (i != std::string::npos) {
        std::cout<<"Detected '..' In Filename, It's Possible That Someone Is Trying To Do Something Nasty\n";
        Filename.erase(i, Pattern.length());
        i = Filename.find(Pattern, i);
    }


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


Controller::Controller(Config::Config &_Config, BG::Common::Logger::LoggingSystem* _Logger) {
    
    // Configure Settings Object
    Settings_ = ConfigureServer(_Config);


    // Add Routes
    EndpointManager_ = std::make_unique<EndpointManager>(_Logger);
    EndpointManager_->AddRoutes(Service_, Server_);

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
        std::cout<<"Loading Private Key From: "<<PrivateKeyURI<<std::endl;
        std::string CertificateURI = std::string("file://") + _Config.CrtFilePath;
        std::cout<<"Loading Certificate From: "<<CertificateURI<<std::endl;
        
        // Setup SSL Settings
        SSLSettings = std::make_shared<restbed::SSLSettings>();
        SSLSettings->set_http_disabled(false);
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
    if (!_Config.UseHTTPS) {
        Settings->set_port(_Config.PortNumber);
    }

    // Set Timeout (Needs To Be Big For Downloading Large Images From Server)
    std::chrono::milliseconds TimeoutDuration_ms(3600000); // 1 Hour
    Settings->set_connection_timeout(TimeoutDuration_ms);
    Settings->set_keep_alive(true);

    // Set CORS Policy, Must Be Allowed, Otherwise API Status Page Doesn't Work Right
    Settings->set_default_header("Access-Control-Allow-Origin", "*"); // We may want to restirct this later on, but since we don't know who's running this, best to leave it open or add a config option

    // Enable Multiple Threads
    Settings->set_worker_limit(std::thread::hardware_concurrency());
    // Return Configured Settings Object
    return Settings;
}


void Controller::StartService() {

    // Also Expose "/.well-known/acme-challenge" for Let's Encrypt to verify from
    std::shared_ptr<restbed::Resource> Resource = std::make_shared<restbed::Resource>();
    Resource->set_path("/.well-known/acme-challenge/{filename: .*}"); // THIS IS BAD, WE DONT STRIP THINGS, CAUSE IM LAZY!!! FIXME!-This still might be bad - we do strip out '..' but still could be bad.
    Resource->set_method_handler( "GET", TextServerHandler);
    Service_.publish(Resource);


    Service_.start(Settings_);

}

Server* Controller::GetServerStruct() {
    return &Server_;
}

void Controller::HangUntilExit() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG