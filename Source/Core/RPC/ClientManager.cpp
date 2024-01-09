#include <RPC/ClientManager.h>


namespace BG {
namespace API {
namespace RPC {


Manager::Manager(std::unique_ptr<BG::Common::Logger::LoggingSystem> _Logger,Config::Config* _Config, Server::Server* _Server) {
    Config_ = _Config;
    Server_ = _Server;
    Logger_ = std::move(_Logger);


    // Initialize Thread Signal
    RequestThreadsExit_ = false;

    // Connect to nes service, start managing service
    std::cout<<"Starting NES Client\n";
    ConnectNES();
    Logger_->Log("Starting NES Client Manager Thread\n",1);
    ConnectionManagerNES_ = std::thread(&Manager::ConnectionManagerNES, this);

    // Populate Server Struct
    Server_->NESClient = NESClient_.get();


}

Manager::~Manager() {

    // Signal to threads to stop
    Logger_->Log("Requesting manager threads exit\n",1);
    RequestThreadsExit_ = true;

    // Join Threads
    Logger_->Log("Joining NES manager thread\n",1);
    ConnectionManagerNES_.join();


}


bool Manager::ConnectNES() {
    NESClient_ = nullptr;

    // Extract NES Client Parameters, Connect, Configure
    std::string NESHost = Config_->NESHost;
    int NESPort = Config_->NESPortNumber;
    int NESTimeout_ms = Config_->NESTimeout_ms;
    
    Logger_->Log("Connecting to NES on port: "+ std::to_string(NESPort)+'n',1);
    Logger_->Log("Connecting to NES on host: "+ NESHost+'\n',1);
    Logger_->Log("Connecting to NES with timeout_ms of: "+std::to_string (NESTimeout_ms)+'\n',1);


    try {
        NESClient_ = std::make_unique<::rpc::client>(NESHost.c_str(), NESPort);
    } catch (std::system_error& e) {
        Logger_->Log("ERR: Cannot find NES host (authoritative)\n",3);
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
        Logger_->Log("[ERR ] Unable to connect to NES service"+'\n',3);
        Server_->NESState = SERVICE_FAILED;
    } else {
        Logger_->Log("[INFO] NES RPC Connection SERVICE_HEALTHY"+'\n',1);
        Server_->NESState = SERVICE_HEALTHY;
    }

    // Check Version again (used as a heartbeat 'isAlive' check)
    std::string NESVersion = "undefined";
    bool Status = NESQueryJSON("GetAPIVersion", &NESVersion);
    if (!Status) {
        Logger_->Log("[ERR ] Failed To Get NES API Version String"+'\n',1);
        return false;
    }

    if (NESVersion != VERSION) {
        Logger_->Log("WARNING: NES/API Version Mismatch! This might make stuff break. NES "+NESVersion+" API "+"2023.11.14"+'\n',2);
        Server_->NESState = SERVICE_VERSION_MISMATCH;
        return false;
    }
    return true;


}

bool Manager::NESQueryJSON(std::string _Route, std::string* _Result) {
    try {
        (*_Result) = NESClient_->call(_Route.c_str()).as<std::string>();
    } catch (::rpc::timeout& e) {
        Logger_->Log("ERR: NES Connection timed out!\n",3);
        Server_->NESState = SERVICE_FAILED;
        return false;
    } catch (::rpc::rpc_error& e) {
        Logger_->Log("ERR: NES remote returned RPC error\n",3);
        Server_->NESState = SERVICE_FAILED;
        return false;
    } catch (std::system_error& e) {
        Logger_->Log("ERR: Cannot talk to NES host\n",3);
        Server_->NESState = SERVICE_CONFIG_ERR;
        return false;
    }
    return true;
}

bool Manager::NESQueryJSON(std::string _Route, std::string _Query, std::string* _Result) {
    try {
        (*_Result) = NESClient_->call(_Route.c_str(), _Query).as<std::string>();
    } catch (::rpc::timeout& e) {
        Logger_->Log("ERR: NES Connection timed out!\n",3);
        Server_->NESState = SERVICE_FAILED;
        return false;
    } catch (::rpc::rpc_error& e) {
        Logger_->Log("ERR: NES remote returned RPC error\n",3);
        Server_->NESState = SERVICE_FAILED;
        return false;
    } catch (std::system_error& e) {
        Logger_->Log("ERR: Cannot talk to NES host\n",3);
        Server_->NESState = SERVICE_CONFIG_ERR;
        return false;
    }
    return true;
}

void Manager::ConnectionManagerNES() {

    Logger_->Log("Started NES Manager Thread\n",3);

    // Enter loop
    while (!RequestThreadsExit_) {

        // Check Version
        bool IsHealthy = RunVersionCheckNES();

        // If not healthy, re-establish connection, retry stuff... For now, nothing...
        if (!IsHealthy) {
            if (!ConnectNES()) {
                Logger_->Log("[ERR ] Failed To Reconnect To NES Service!\n",3);            }
        }

        // Wait 1000ms before polling again
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    }
    Logger_->Log("Exiting NES Manager Thread\n",1);
}

}; // Close Namespace DB
}; // Close Namespace API
}; // Close Namespace BG