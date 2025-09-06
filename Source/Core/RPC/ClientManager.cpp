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

    // Start NES Client
    Logger_->Log("Starting NES Client", 1);
    ConnectNES();
    Logger_->Log("Starting NES Client Manager Thread", 1);
    ConnectionManagerNES_ = std::thread(&Manager::ConnectionManagerNES, this);

    // Populate Server Struct
    Server_->NESClient = NESClient_;
    Server_->IsNESClientHealthy_ = &IsNESClientHealthy_;

    // Start EVM Client
    Logger_->Log("Starting EVM Client", 1);
    ConnectEVM();
    Logger_->Log("Starting EVM Client Manager Thread", 1);
    ConnectionManagerEVM_ = std::thread(&Manager::ConnectionManagerEVM, this);

    // Populate Server Struct
    Server_->EVMClient = EVMClient_;
    Server_->IsEVMClientHealthy_ = &IsEVMClientHealthy_;
}

Manager::~Manager() {
    Logger_->Log("Requesting manager threads exit", 1);
    RequestThreadsExit_ = true;

    if (ConnectionManagerNES_.joinable()) {
        ConnectionManagerNES_.join();
    }

    if (ConnectionManagerEVM_.joinable()) {
        ConnectionManagerEVM_.join();
    }
}

void Manager::SetEVMCallbackInfo() {
    // Only attempt if client exists and is healthy
    if (!EVMClient_ || !IsEVMClientHealthy_.load()) return;

    nlohmann::json Query;
    Query["CallbackHost"] = Config_->RPCCallbackHost;
    Query["CallbackPort"] = Config_->RPCCallbackPort;
    std::string QueryStr = Query.dump();

    std::string Result;
    EVMQueryJSON("SetCallback", QueryStr, &Result, true); // Fail silently
}

void Manager::SetNESCallbackInfo() {
    // Only attempt if client exists and is healthy
    if (!NESClient_ || !IsNESClientHealthy_.load()) return;

    nlohmann::json Query;
    Query["CallbackHost"] = Config_->RPCCallbackHost;
    Query["CallbackPort"] = Config_->RPCCallbackPort;
    std::string QueryStr = Query.dump();

    std::string Result;
    NESQueryJSON("SetCallback", QueryStr, &Result, true); // Fail silently
}

bool Manager::ConnectEVM() {
    IsEVMClientHealthy_.store(false);
    EVMClient_ = nullptr;

    std::string EVMHost = Config_->EVMHost;
    int EVMPort = Config_->EVMPortNumber;
    int EVMTimeout_ms = Config_->EVMTimeout_ms;

    // Logger_->Log("Connecting to EVM on port: " + std::to_string(EVMPort), 5);
    // Logger_->Log("Connecting to EVM on host: " + EVMHost, 5);
    // Logger_->Log("Connecting to EVM with timeout_ms of: " + std::to_string(EVMTimeout_ms), 5);

    try {
        EVMClient_ = std::make_shared<::rpc::client>(EVMHost.c_str(), EVMPort);
        Server_->EVMClient = EVMClient_;
    } catch (...) {
        Server_->EVMState = SERVICE_CONFIG_ERR;
        return false;
    }

    EVMClient_->set_timeout(EVMTimeout_ms);

    bool status = RunVersionCheckEVM();
    if (status) {
        IsEVMClientHealthy_.store(true);
        SetEVMCallbackInfo(); // Only set callback if healthy
    }

    return status;
}

bool Manager::ConnectNES() {
    IsNESClientHealthy_.store(false);
    NESClient_ = nullptr;

    std::string NESHost = Config_->NESHost;
    int NESPort = Config_->NESPortNumber;
    int NESTimeout_ms = Config_->NESTimeout_ms;

    // Logger_->Log("Connecting to NES on port: " + std::to_string(NESPort), 5);
    // Logger_->Log("Connecting to NES on host: " + NESHost, 5);
    // Logger_->Log("Connecting to NES with timeout_ms of: " + std::to_string(NESTimeout_ms), 5);

    try {
        NESClient_ = std::make_shared<::rpc::client>(NESHost.c_str(), NESPort);
        Server_->NESClient = NESClient_;
    } catch (...) {
        Server_->NESState = SERVICE_CONFIG_ERR;
        return false;
    }

    NESClient_->set_timeout(NESTimeout_ms);

    bool status = RunVersionCheckNES();
    if (status) {
        IsNESClientHealthy_.store(true);
        SetNESCallbackInfo(); // Only set callback if healthy
    }

    return status;
}

bool Manager::RunVersionCheckEVM() {
    std::string EVMVersion = "undefined";
    bool status = EVMQueryJSON("GetAPIVersion", &EVMVersion, true);

    if (!status) {
        Server_->EVMState = SERVICE_FAILED;
        IsEVMClientHealthy_.store(false);
        return false;
    }

    if (EVMVersion != VERSION) {
        Logger_->Log("EVM/API Version Mismatch! This might make stuff break. EVM " + EVMVersion + " API " + VERSION, 9);
        Server_->EVMState = SERVICE_VERSION_MISMATCH;
        IsEVMClientHealthy_.store(false);
        return false;
    }

    Server_->EVMState = SERVICE_HEALTHY;
    IsEVMClientHealthy_.store(true);
    return true;
}

bool Manager::RunVersionCheckNES() {
    std::string NESVersion = "undefined";
    bool status = NESQueryJSON("GetAPIVersion", &NESVersion, true);

    if (!status) {
        Server_->NESState = SERVICE_FAILED;
        IsNESClientHealthy_.store(false);
        return false;
    }

    if (NESVersion != VERSION) {
        Logger_->Log("NES/API Version Mismatch! This might make stuff break. NES " + NESVersion + " API " + VERSION, 9);
        Server_->NESState = SERVICE_VERSION_MISMATCH;
        IsNESClientHealthy_.store(false);
        return false;
    }

    Server_->NESState = SERVICE_HEALTHY;
    IsNESClientHealthy_.store(true);
    return true;
}

// -----------------------------
// RPC Query Functions (Silent on Failure)
// -----------------------------

bool Manager::EVMQueryJSON(std::string _Route, std::string* _Result, bool _ForceQuery) {
    if (!_ForceQuery && (!IsEVMClientHealthy_.load() || !EVMClient_)) {
        return false;
    }
    try {
        (*_Result) = EVMClient_->call(_Route.c_str()).as<std::string>();
        return true;
    } catch (...) {
        return false;
    }
}

bool Manager::EVMQueryJSON(std::string _Route, std::string _Query, std::string* _Result, bool _ForceQuery) {
    if (!_ForceQuery && (!IsEVMClientHealthy_.load() || !EVMClient_)) {
        return false;
    }
    try {
        (*_Result) = EVMClient_->call(_Route.c_str(), _Query).as<std::string>();
        return true;
    } catch (...) {
        return false;
    }
}

bool Manager::NESQueryJSON(std::string _Route, std::string* _Result, bool _ForceQuery) {
    if (!_ForceQuery && (!IsNESClientHealthy_.load() || !NESClient_)) {
        return false;
    }
    try {
        (*_Result) = NESClient_->call(_Route.c_str()).as<std::string>();
        return true;
    } catch (...) {
        return false;
    }
}

bool Manager::NESQueryJSON(std::string _Route, std::string _Query, std::string* _Result, bool _ForceQuery) {
    if (!_ForceQuery && (!IsNESClientHealthy_.load() || !NESClient_)) {
        return false;
    }
    try {
        (*_Result) = NESClient_->call(_Route.c_str(), _Query).as<std::string>();
        return true;
    } catch (...) {
        return false;
    }
}

// -----------------------------
// Connection Manager Threads
// -----------------------------

void Manager::ConnectionManagerEVM() {
    Logger_->Log("Started EVM Manager Thread", 3);
    int lastState = -1;

    while (!RequestThreadsExit_) {
        // Update state via health check or assume failed if no client
        if (EVMClient_) {
            RunVersionCheckEVM();
        } else {
            Server_->EVMState = SERVICE_FAILED;
        }

        int currentState = Server_->EVMState;

        if (currentState != lastState) {
            switch (currentState) {
                case SERVICE_HEALTHY:
                    Logger_->Log("EVM Connection now HEALTHY", 1);
                    break;
                case SERVICE_FAILED:
                    Logger_->Log("EVM Connection now FAILED", 3);
                    break;
                case SERVICE_VERSION_MISMATCH:
                    Logger_->Log("EVM Connection now VERSION_MISMATCH", 9);
                    break;
                case SERVICE_CONFIG_ERR:
                    Logger_->Log("EVM Connection now CONFIG_ERR", 9);
                    break;
                default:
                    Logger_->Log("EVM Connection now in UNKNOWN state", 9);
            }
            lastState = currentState;
        }

        // Reconnect only if not healthy
        if (currentState != SERVICE_HEALTHY) {
            ConnectEVM();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    Logger_->Log("Exiting EVM Manager Thread", 1);
}

void Manager::ConnectionManagerNES() {
    Logger_->Log("Started NES Manager Thread", 3);
    int lastState = -1;

    while (!RequestThreadsExit_) {
        // Update state via health check or assume failed if no client
        if (NESClient_) {
            RunVersionCheckNES();
        } else {
            Server_->NESState = SERVICE_FAILED;
        }

        int currentState = Server_->NESState;

        if (currentState != lastState) {
            switch (currentState) {
                case SERVICE_HEALTHY:
                    Logger_->Log("NES Connection now HEALTHY", 1);
                    break;
                case SERVICE_FAILED:
                    Logger_->Log("NES Connection now FAILED", 3);
                    break;
                case SERVICE_VERSION_MISMATCH:
                    Logger_->Log("NES Connection now VERSION_MISMATCH", 9);
                    break;
                case SERVICE_CONFIG_ERR:
                    Logger_->Log("NES Connection now CONFIG_ERR", 9);
                    break;
                default:
                    Logger_->Log("NES Connection now in UNKNOWN state", 9);
            }
            lastState = currentState;
        }

        // Reconnect only if not healthy
        if (currentState != SERVICE_HEALTHY) {
            ConnectNES();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    Logger_->Log("Exiting NES Manager Thread", 1);
}

}; // namespace RPC
}; // namespace API
}; // namespace BG