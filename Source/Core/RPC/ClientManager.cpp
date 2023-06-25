#include <RPC/ClientManager.h>


namespace BG {
namespace API {
namespace RPC {


Manager::Manager(Config::Config* _Config, Server::Server* _Server) {
    Config_ = _Config;
    Server_ = _Server;

    // Connect to nes service
    ConnectNES();


}

Manager::~Manager() {

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
        NESClient_ = std::make_unique<::rpc::client>(NESHost, NESPort);
    } catch (std::system_error& e) {
        std::cout<<"ERR: Cannot find NES host (authoritative)\n";
        Server_->NESState = SERVICE_CONFIG_ERR;
        return false;
    }
    NESClient_->set_timeout(NESTimeout_ms);

    // Call GetVersion On Remote - allows us to check that versions match, but also ensures the connection is ready
    std::string NESVersion = "undefined";
    try {
        NESVersion = NESClient_->call("GetVersion").as<std::string>();
    } catch (::rpc::timeout& e) {
        std::cout<<"ERR: NES Connection timed out!\n";
        Server_->NESState = SERVICE_FAILED;
        return false;
    }
    
    if (NESVersion != VERSION) {
        std::cout<<"WARNING: NES/API Version Mismatch! This might make stuff break. NES "<<NESVersion<<" API "<<VERSION<<std::endl;
        Server_->NESState = SERVICE_DEGRADED;
    }

    // Finally, Update our internal status of how the connection is doing
    ::rpc::client::connection_state NESStatus = NESClient_->get_connection_state();
    if (NESStatus != ::rpc::client::connection_state::connected) {
        Server_->NESState = SERVICE_FAILED;
        return false;
    } else {
        Server_->NESState = SERVICE_HEALTHY;
        return true;
    }

}

}; // Close Namespace DB
}; // Close Namespace API
}; // Close Namespace BG