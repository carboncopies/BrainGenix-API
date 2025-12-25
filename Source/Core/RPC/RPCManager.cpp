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
#include <Resource/CentralizedRequestHandler/CentralizedRequestHandler.h>
#include <nlohmann/json.hpp>




RPCManager::RPCManager(ConfigParser* _Config, BG::Common::Logger::LoggingSystem* _Logger, Server* _Server) {

    Logger_ = _Logger;
    Server_ = _Server;

    // Initialize Server
    std::string ServerHost = _Config->GetString("RPCCallbackHost", "localhost");
    int ServerPort = _Config->GetInt("RPCCallbackPort", 7999);

    // Create a unique pointer to the RPC server and initialize it with the provided port number
    RPCServer_ = std::make_unique<rpc::server>(ServerPort);

    // Register Basic Routes
    // Add predefined routes to the RPC server
    AddRoute("GetAPIVersion", _Logger, &GetAPIVersion);
    AddRoute("Echo", _Logger, &Echo);
    AddRoute("NES", Logger_, [this](std::string RequestJSON){ return NESRequest(RequestJSON);});
    AddRoute("EVM", Logger_, [this](std::string RequestJSON){ return EVMRequest(RequestJSON);});
    
    // Register QueryBackendService route for centralized request handling
    AddRoute("QueryBackendService", Logger_, [this](std::string RequestJSON) {
        try {
            nlohmann::json request = nlohmann::json::parse(RequestJSON);
            
            // Extract required fields from JSON
            std::string targetService = request.value("TargetService", "");
            std::string rpcQuery = request.value("RPCQuery", "");
            std::string queryContent = request.value("QueryContent", "");
            
            // Call the centralized request handler
            return CentralizedRequestHandler::RouteToBackendService(
                Server_, 
                Logger_, 
                targetService, 
                rpcQuery, 
                queryContent
            );
        } catch (const std::exception& e) {
            if(Logger_) {
                Logger_->Log("[RPCManager] Error parsing QueryBackendService request: " + std::string(e.what()), 7);
            }
            nlohmann::json errorResponse;
            errorResponse["error"] = "Invalid request format: " + std::string(e.what());
            return errorResponse.dump();
        }
    });

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

}




// This method is for internal requests to NES (such as from EVM)
std::string RPCManager::NESRequest(std::string _JSONRequest, int _SimulationIDOverride) { // Generic JSON-based API requests.

    std::string UpstreamResponseStr = "";
    bool Status = NESQueryJSON(Server_->NESClient, Server_->IsNESClientHealthy_, "NES", _JSONRequest, &UpstreamResponseStr); 

    return UpstreamResponseStr;

}

// This method is for internal requests to EVM (such as from NES)
std::string RPCManager::EVMRequest(std::string _JSONRequest, int _SimulationIDOverride) { // Generic JSON-based API requests.

    std::string UpstreamResponseStr = "";
    bool Status = EVMQueryJSON(Server_->EVMClient, Server_->IsEVMClientHealthy_, "EVM", _JSONRequest, &UpstreamResponseStr); 

    return UpstreamResponseStr;

}

