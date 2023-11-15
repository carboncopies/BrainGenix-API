#include <RPC/ClientManager.h>


namespace BG {
namespace API {
namespace RPC {


Manager::Manager(Config::Config* _Config, Server::Server* _Server) {
    Config_ = _Config;
    Server_ = _Server;

    // Initialize Thread Signal
    RequestThreadsExit_ = false;

    // Connect to nes service, start managing service
    std::cout<<"Starting NES Client\n";
    ConnectNES();
    std::cout<<"Starting NES Client Manager Thread\n";
    ConnectionManagerNES_ = std::thread(&Manager::ConnectionManagerNES, this);

    // Populate Server Struct
    Server_->NESClient = NESClient_.get();


}

Manager::~Manager() {

    // Signal to threads to stop
    std::cout<<"Requesting manager threads exit\n";
    RequestThreadsExit_ = true;

    // Join Threads
    std::cout<<"Joining NES manager thread\n";
    ConnectionManagerNES_.join();


}


bool Manager::ConnectNES() {
    NESClient_ = nullptr;

    // Extract NES Client Parameters, Connect, Configure
    std::string NESHost = Config_->NESHost;
    int NESPort = Config_->NESPortNumber;
    int NESTimeout_ms = Config_->NESTimeout_ms;
    
    std::cout<<"Connecting to NES on port: "<<NESPort<<std::endl;
    std::cout<<"Connecting to NES on host: "<<NESHost<<std::endl;
    std::cout<<"Connecting to NES with timeout_ms of: "<<NESTimeout_ms<<std::endl;

    try {
        NESClient_ = std::make_unique<::rpc::client>(NESHost.c_str(), NESPort);
    } catch (std::system_error& e) {
        std::cout<<"ERR: Cannot find NES host (authoritative)\n";
        Server_->NESState = SERVICE_CONFIG_ERR;
        return false;
    }
    NESClient_->set_timeout(NESTimeout_ms);

    // Call GetVersion On Remote - allows us to check that versions match, but also ensures the connection is ready
    return RunVersionCheckNES();

}

bool Manager::RunVersionCheckNES() {

    // Run a query to force it to connect (or fail)
    std::string Temp;
    NESQueryJSON("GetAPIVersion", &Temp);
    

    // Update our internal status of how the connection is doing
    ::rpc::client::connection_state NESStatus = NESClient_->get_connection_state();
    if (NESStatus != ::rpc::client::connection_state::connected) {
        std::cout<<"[ERR ] Unable to connect to NES service"<<std::endl;
        Server_->NESState = SERVICE_FAILED;
    } else {
        std::cout<<"[INFO] NES RPC Connection SERVICE_HEALTHY"<<std::endl;
        Server_->NESState = SERVICE_HEALTHY;
    }

    // Check Version again (used as a heartbeat 'isAlive' check)
    std::string NESVersion = "undefined";
    bool Status = NESQueryJSON("GetAPIVersion", &NESVersion);
    if (!Status) {
        std::cout<<"[ERR ] Failed To Get NES API Version String"<<std::endl;
        return false;
    }

    if (NESVersion != "2023.11.14") {
        std::cout<<"WARNING: NES/API Version Mismatch! This might make stuff break. NES "<<NESVersion<<" API "<<"2023.11.14"<<std::endl;
        Server_->NESState = SERVICE_VERSION_MISMATCH;
        return false;
    }
    return true;


}

bool Manager::NESQueryJSON(std::string _Route, std::string* _Result) {
    try {
        (*_Result) = NESClient_->call(_Route.c_str()).as<std::string>();
    } catch (::rpc::timeout& e) {
        std::cout<<"ERR: NES Connection timed out!\n";
        Server_->NESState = SERVICE_FAILED;
        return false;
    } catch (::rpc::rpc_error& e) {
        std::cout<<"ERR: NES remote returned RPC error\n";
        Server_->NESState = SERVICE_FAILED;
        return false;
    } catch (std::system_error& e) {
        std::cout<<"ERR: Cannot talk to NES host\n";
        Server_->NESState = SERVICE_CONFIG_ERR;
        return false;
    }
    return true;
}

bool Manager::NESQueryJSON(std::string _Route, std::string _Query, std::string* _Result) {
    try {
        (*_Result) = NESClient_->call(_Route.c_str(), _Query).as<std::string>();
    } catch (::rpc::timeout& e) {
        std::cout<<"ERR: NES Connection timed out!\n";
        Server_->NESState = SERVICE_FAILED;
        return false;
    } catch (::rpc::rpc_error& e) {
        std::cout<<"ERR: NES remote returned RPC error\n";
        Server_->NESState = SERVICE_FAILED;
        return false;
    } catch (std::system_error& e) {
        std::cout<<"ERR: Cannot talk to NES host\n";
        Server_->NESState = SERVICE_CONFIG_ERR;
        return false;
    }
    return true;
}

void Manager::ConnectionManagerNES() {

    std::cout<<"Started NES Manager Thread\n";

    // Enter loop
    while (!RequestThreadsExit_) {

        // Check Version
        bool IsHealthy = RunVersionCheckNES();

        // If not healthy, re-establish connection, retry stuff... For now, nothing...
        if (!IsHealthy) {
            if (!ConnectNES()) {
                std::cout<<"[ERR ] Failed To Reconnect To NES Service!"<<std::endl;
            }
        }

        // Wait 1000ms before polling again
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    }

    std::cout<<"Exiting NES Manager Thread\n";

}

}; // Close Namespace DB
}; // Close Namespace API
}; // Close Namespace BG
