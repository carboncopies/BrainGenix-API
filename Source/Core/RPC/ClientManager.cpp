#include <nlohmann/json.hpp>

#include <RPC/ClientManager.h>


namespace BG {
namespace API {
namespace RPC {


Manager::Manager(BG::Common::Logger::LoggingSystem* _Logger, Config::Config* _Config, Server::Server* _Server) {
    Config_ = _Config;
    Server_ = _Server;
    Logger_ = _Logger;


    // Initialize Thread Signal
    RequestThreadsExit_ = false;

    // Connect to nes service, start managing service
    Logger_->Log("Starting NES Client", 1);
    ConnectNES();
    Logger_->Log("Starting NES Client Manager Thread",1);
    ConnectionManagerNES_ = std::thread(&Manager::ConnectionManagerNES, this);

    // Populate Server Struct
    Server_->NESClient = NESClient_;
    Server_->IsNESClientHealthy_ = &IsNESClientHealthy_;


    // Connect to EVM service, start managing service
    Logger_->Log("Starting EVM Client", 1);
    ConnectEVM();
    Logger_->Log("Starting EVM Client Manager Thread",1);
    ConnectionManagerEVM_ = std::thread(&Manager::ConnectionManagerEVM, this);

    // Populate Server Struct
    Server_->EVMClient = EVMClient_;
    Server_->IsEVMClientHealthy_ = &IsEVMClientHealthy_;

}

Manager::~Manager() {

    // Signal to threads to stop
    Logger_->Log("Requesting manager threads exit",1);
    RequestThreadsExit_ = true;

    // Join Threads
    Logger_->Log("Joining NES manager thread",1);
    ConnectionManagerNES_.join();

    Logger_->Log("Joining EVM manager thread",1);
    ConnectionManagerEVM_.join();

}


void Manager::SetNESCallbackInfo() {
    nlohmann::json Query;
    Query["CallbackHost"] = Config_->RPCCallbackHost;
    Query["CallbackPort"] = Config_->RPCCallbackPort;
    std::string QueryStr = Query.dump();

    std::string Result;
    NESQueryJSON("SetCallback", QueryStr, &Result, true);

    Logger_->Log("Set NES RPC Callback", 3);
}

void Manager::SetEVMCallbackInfo() {
    nlohmann::json Query;
    Query["CallbackHost"] = Config_->RPCCallbackHost;
    Query["CallbackPort"] = Config_->RPCCallbackPort;
    std::string QueryStr = Query.dump();

    std::string Result;
    EVMQueryJSON("SetCallback", QueryStr, &Result, true);

    Logger_->Log("Set EVM RPC Callback", 3);
}

bool Manager::ConnectNES() {
    IsNESClientHealthy_.store(false);
    NESClient_ = nullptr;

    // Extract NES Client Parameters, Connect, Configure
    std::string NESHost = Config_->NESHost;
    int NESPort = Config_->NESPortNumber;
    int NESTimeout_ms = Config_->NESTimeout_ms;
    
    Logger_->Log("Connecting to NES on port: " + std::to_string(NESPort), 1);
    Logger_->Log("Connecting to NES on host: " + NESHost, 1);
    Logger_->Log("Connecting to NES with timeout_ms of: " + std::to_string(NESTimeout_ms), 1);


    try {
        NESClient_ = std::make_shared<::rpc::client>(NESHost.c_str(), NESPort);
        Server_->NESClient = NESClient_; // Update Server's client pointer
    } catch (std::system_error& e) {
        Logger_->Log("Cannot find NES host (authoritative)", 9);
        Server_->NESState = SERVICE_CONFIG_ERR;
        return false;
    }
    NESClient_->set_timeout(NESTimeout_ms);

    // Call GetVersion On Remote - allows us to check that versions match, but also ensures the connection is ready
    bool Status = RunVersionCheckNES();
    if (Status) {
        IsNESClientHealthy_.store(true);
    }

    Logger_->Log("Setting NES RPC Callback", 3);
    SetNESCallbackInfo();
    

    return Status;

}

bool Manager::ConnectEVM() {
    IsEVMClientHealthy_.store(false);
    EVMClient_ = nullptr;

    // Extract EVM Client Parameters, Connect, Configure
    std::string EVMHost = Config_->EVMHost;
    int EVMPort = Config_->EVMPortNumber;
    int EVMTimeout_ms = Config_->EVMTimeout_ms;
    
    Logger_->Log("Connecting to EVM on port: " + std::to_string(EVMPort), 1);
    Logger_->Log("Connecting to EVM on host: " + EVMHost, 1);
    Logger_->Log("Connecting to EVM with timeout_ms of: " + std::to_string(EVMTimeout_ms), 1);


    try {
        EVMClient_ = std::make_shared<::rpc::client>(EVMHost.c_str(), EVMPort);
        Server_->EVMClient = EVMClient_; // Update Server's client pointer
    } catch (std::system_error& e) {
        Logger_->Log("Cannot find EVM host (authoritative)", 9);
        Server_->EVMState = SERVICE_CONFIG_ERR;
        return false;
    }
    EVMClient_->set_timeout(EVMTimeout_ms);

    // Call GetVersion On Remote - allows us to check that versions match, but also ensures the connection is ready
    bool Status = RunVersionCheckEVM();
    if (Status) {
        IsEVMClientHealthy_.store(true);
    }

    Logger_->Log("Setting EVM RPC Callback", 3);
    SetEVMCallbackInfo();
    
    return Status;

}



bool Manager::RunVersionCheckNES() {

    // Run a query to force it to connect (or fail)
    std::string Temp;
    NESQueryJSON("GetAPIVersion", &Temp);
    

    // Update our internal status of how the connection is doing
    ::rpc::client::connection_state NESStatus = NESClient_->get_connection_state();
    if (NESStatus != ::rpc::client::connection_state::connected) {
        Logger_->Log("Unable to connect to NES service", 3);
        Server_->NESState = SERVICE_FAILED;
        IsNESClientHealthy_.store(false);
    } else {
        Logger_->Log("NES RPC Connection SERVICE_HEALTHY", 1);
        Server_->NESState = SERVICE_HEALTHY;
    }

    // Check Version again (used as a heartbeat 'isAlive' check)
    std::string NESVersion = "undefined";
    bool Status = NESQueryJSON("GetAPIVersion", &NESVersion, true);
    if (!Status) {
        Logger_->Log("Failed To Get NES API Version String", 1);
        return false;
    }

    if (NESVersion != VERSION) {
        Logger_->Log("NES/API Version Mismatch! This might make stuff break. NES " + NESVersion + " API " + VERSION, 9);
        Server_->NESState = SERVICE_VERSION_MISMATCH;
        return false;
    }
    return true;


}

bool Manager::RunVersionCheckEVM() {

    // Run a query to force it to connect (or fail)
    std::string Temp;
    EVMQueryJSON("GetAPIVersion", &Temp);
    

    // Update our internal status of how the connection is doing
    ::rpc::client::connection_state EVMStatus = EVMClient_->get_connection_state();
    if (EVMStatus != ::rpc::client::connection_state::connected) {
        Logger_->Log("Unable to connect to EVM service", 3);
        Server_->EVMState = SERVICE_FAILED;
        IsEVMClientHealthy_.store(false);
    } else {
        Logger_->Log("EVM RPC Connection SERVICE_HEALTHY", 1);
        Server_->EVMState = SERVICE_HEALTHY;
    }

    // Check Version again (used as a heartbeat 'isAlive' check)
    std::string EVMVersion = "undefined";
    bool Status = EVMQueryJSON("GetAPIVersion", &EVMVersion, true);
    if (!Status) {
        Logger_->Log("Failed To Get EVM API Version String", 1);
        return false;
    }

    if (EVMVersion != VERSION) {
        Logger_->Log("EVM/API Version Mismatch! This might make stuff break. EVM " + EVMVersion + " API " + VERSION, 9);
        Server_->EVMState = SERVICE_VERSION_MISMATCH;
        return false;
    }
    return true;


}

bool Manager::NESQueryJSON(std::string _Route, std::string* _Result, bool _ForceQuery) {
    if (!_ForceQuery && !IsNESClientHealthy_.load()) {
        return false;
    }
    try {
        (*_Result) = NESClient_->call(_Route.c_str()).as<std::string>();
    } catch (::rpc::timeout& e) {
        Logger_->Log("NES Connection timed out!",3);
        Server_->NESState = SERVICE_FAILED;
        return false;
    } catch (::rpc::rpc_error& e) {
        Logger_->Log("NES remote returned RPC error",3);
        Server_->NESState = SERVICE_FAILED;
        return false;
    } catch (std::system_error& e) {
        Logger_->Log("Cannot talk to NES host",3);
        Server_->NESState = SERVICE_CONFIG_ERR;
        return false;
    }
    return true;
}

bool Manager::NESQueryJSON(std::string _Route, std::string _Query, std::string* _Result, bool _ForceQuery) {
    if (!_ForceQuery && !IsNESClientHealthy_.load()) {
        return false;
    }
    try {
        (*_Result) = NESClient_->call(_Route.c_str(), _Query).as<std::string>();
    } catch (::rpc::timeout& e) {
        Logger_->Log("NES Connection timed out!",3);
        Server_->NESState = SERVICE_FAILED;
        return false;
    } catch (::rpc::rpc_error& e) {
        Logger_->Log("NES remote returned RPC error",3);
        Server_->NESState = SERVICE_FAILED;
        return false;
    } catch (std::system_error& e) {
        Logger_->Log("Cannot talk to NES host",3);
        Server_->NESState = SERVICE_CONFIG_ERR;
        return false;
    }
    return true;
}


bool Manager::EVMQueryJSON(std::string _Route, std::string* _Result, bool _ForceQuery) {
    if (!_ForceQuery && !IsEVMClientHealthy_.load()) {
        return false;
    }
    try {
        (*_Result) = EVMClient_->call(_Route.c_str()).as<std::string>();
    } catch (::rpc::timeout& e) {
        Logger_->Log("EVM Connection timed out!",3);
        Server_->EVMState = SERVICE_FAILED;
        return false;
    } catch (::rpc::rpc_error& e) {
        Logger_->Log("EVM remote returned RPC error",3);
        Server_->EVMState = SERVICE_FAILED;
        return false;
    } catch (std::system_error& e) {
        Logger_->Log("Cannot talk to EVM host",3);
        Server_->EVMState = SERVICE_CONFIG_ERR;
        return false;
    }
    return true;
}

bool Manager::EVMQueryJSON(std::string _Route, std::string _Query, std::string* _Result, bool _ForceQuery) {
    if (!_ForceQuery && !IsEVMClientHealthy_.load()) {
        return false;
    }
    try {
        (*_Result) = EVMClient_->call(_Route.c_str(), _Query).as<std::string>();
    } catch (::rpc::timeout& e) {
        Logger_->Log("EVM Connection timed out!",3);
        Server_->EVMState = SERVICE_FAILED;
        return false;
    } catch (::rpc::rpc_error& e) {
        Logger_->Log("EVM remote returned RPC error",3);
        Server_->EVMState = SERVICE_FAILED;
        return false;
    } catch (std::system_error& e) {
        Logger_->Log("Cannot talk to EVM host",3);
        Server_->EVMState = SERVICE_CONFIG_ERR;
        return false;
    }
    return true;
}


void Manager::ConnectionManagerNES() {

    Logger_->Log("Started NES Manager Thread",3);

    // Enter loop
    while (!RequestThreadsExit_) {

        // Check Version
        bool IsHealthy = RunVersionCheckNES();

        // If not healthy, re-establish connection, retry stuff... For now, nothing...
        if (!IsHealthy) {
            if (!ConnectNES()) {
                Logger_->Log("Failed To Reconnect To NES Service",3);            }
        }

        // Wait 1000ms before polling again
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    }
    Logger_->Log("Exiting NES Manager Thread",1);
}

void Manager::ConnectionManagerEVM() {

    Logger_->Log("Started EVM Manager Thread",3);

    // Enter loop
    while (!RequestThreadsExit_) {

        // Check Version
        bool IsHealthy = RunVersionCheckEVM();

        // If not healthy, re-establish connection, retry stuff... For now, nothing...
        if (!IsHealthy) {
            if (!ConnectEVM()) {
                Logger_->Log("Failed To Reconnect To EVM Service",3);            }
        }

        // Wait 1000ms before polling again
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    }
    Logger_->Log("Exiting EVM Manager Thread",1);
}

}; // Close Namespace DB
}; // Close Namespace API
}; // Close Namespace BG