//=================================//
// This file is part of BrainGenix //
//=================================//


// Standard Libraries (BG convention: use <> instead of "")
#include <thread>

// Third-Party Libraries (BG convention: use <> instead of "")

// Internal Libraries (BG convention: use <> instead of "")

#include <RPC/RPCManager.h>

#include <RPC/RPCHandlerHelper.h>
#include <RPC/APIStatusCode.h>


namespace BG {
namespace API {
namespace API {


RPCManager::RPCManager(Config::Config* _Config, BG::Common::Logger::LoggingSystem* _Logger, Server::Server* _Server) {

    Logger_ = _Logger;
    Server_ = _Server;

    // Initialize Server
    std::string ServerHost = _Config->RPCCallbackHost;
    int ServerPort = _Config->RPCCallbackPort;

    // Create a unique pointer to the RPC server and initialize it with the provided port number
    RPCServer_ = std::make_unique<rpc::server>(ServerPort);

    // Register Basic Routes
    // Add predefined routes to the RPC server
    AddRoute("GetAPIVersion", _Logger, &GetAPIVersion);
    AddRoute("Echo", _Logger, &Echo);
    AddRoute("NES", Logger_, [this](std::string RequestJSON){ return NESRequest(RequestJSON);});
    AddRoute("EVM", Logger_, [this](std::string RequestJSON){ return EVMRequest(RequestJSON);});

    int ThreadCount = std::thread::hardware_concurrency();
    _Logger->Log("Starting RPC Server With '" + std::to_string(ThreadCount) + "' Threads", 5);
    _Logger->Log("RPC Server On Host '" + ServerHost + "'", 4);
    _Logger->Log("RPC Server On Port '" + std::to_string(ServerPort) + "'", 4);
    
    // Start the RPC server asynchronously with the specified thread count
    RPCServer_->async_run(ThreadCount);
}

RPCManager::~RPCManager() {
    // Destructor
    // No explicit cleanup needed as smart pointers manage the RPC server's memory
}



void RPCManager::AddRoute(std::string _RouteHandle, std::function<std::string(std::string _JSONRequest)> _Function) {
    Logger_->Log("Registering Callback For Route '" + _RouteHandle + "'", 4);
    RequestHandlers_.insert(std::pair<std::string, std::function<std::string(std::string _JSONRequest)>>(_RouteHandle, _Function));
    // RouteAndHandler Handler;
    // Handler.Route_ = _RouteHandle;
    // Handler.Handler_ = _Function;
    // AddRequestHandler(_RouteHandle, Handler);
}





std::string RPCManager::NESRequest(std::string _JSONRequest, int _SimulationIDOverride) { // Generic JSON-based API requests.

    // Logger_->Log("Received NES Request: " + _JSONRequest, 0); // For DEBUGGING


    std::string UpstreamResponseStr = "";
    bool Status = Util::NESQueryJSON(Server_->NESClient, Server_->IsNESClientHealthy_, "NES", _JSONRequest, &UpstreamResponseStr); 

    // Logger_->Log("Responding: " + UpstreamResponseStr, 0); // For DEBUGGING


    return UpstreamResponseStr;

}

std::string RPCManager::EVMRequest(std::string _JSONRequest, int _SimulationIDOverride) { // Generic JSON-based API requests.

    // Logger_->Log("Received EVM Request: " + _JSONRequest, 0); // For DEBUGGING


    std::string UpstreamResponseStr = "";
    bool Status = Util::EVMQueryJSON(Server_->EVMClient, Server_->IsEVMClientHealthy_, "EVM", _JSONRequest, &UpstreamResponseStr); 

    // Logger_->Log("Responding: " + UpstreamResponseStr, 0); // For DEBUGGING


    return UpstreamResponseStr;

}


}; // Close Namespace API
}; // Close Namespace API
}; // Close Namespace BG