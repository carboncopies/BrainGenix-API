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

#include <Resource/VSDA/IPDetector.h>


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

}




// This method is for internal requests to NES (such as from EVM)
std::string RPCManager::NESRequest(std::string _JSONRequest, int _SimulationIDOverride) { // Generic JSON-based API requests.

    std::string UpstreamResponseStr = "";
    bool Status = Util::NESQueryJSON(Server_->NESClient, Server_->IsNESClientHealthy_, "NES", _JSONRequest, &UpstreamResponseStr); 

    return UpstreamResponseStr;

}

// This method is for internal requests to EVM (such as from NES)
std::string RPCManager::EVMRequest(std::string _JSONRequest, int _SimulationIDOverride) { // Generic JSON-based API requests.

    std::string UpstreamResponseStr = "";
    bool Status = Util::EVMQueryJSON(Server_->EVMClient, Server_->IsEVMClientHealthy_, "EVM", _JSONRequest, &UpstreamResponseStr); 

    return UpstreamResponseStr;

}


// In your RPC binding section, add these endpoints:
void RPCManager::BindVSDAEndpoints(BG::API::VSDA::Manager* vsdaManager) {
    // Register node endpoint
    RPCServer_->bind("RegisterNode", [vsdaManager](const std::string& j) {
        auto obj = nlohmann::json::parse(j);
        std::string nodeId = obj["id"];
        std::string host = obj["host"];
        int port = obj["port"];
        
        // If host is localhost, use the client's IP
        if (host == "127.0.0.1" || host == "localhost") {
            // try {
                // Get client IP from current session
                // h/ost = rpc::this_session().get_raw_ip();
            // } catch (...) {
                host = IPUtils::GetPrimaryIP();
            // }
        }
        
        return vsdaManager->RegisterNode(nodeId, host, port).dump();
    });
    
    // Heartbeat endpoint
    RPCServer_->bind("Heartbeat", [vsdaManager](const std::string& id) {
        return vsdaManager->CheckIn(id, "", 0, false).dump();
    });
    
    // Leader heartbeat endpoint
    RPCServer_->bind("LeaderHeartbeat", [vsdaManager](const std::string& leaderId, 
                                                  const std::string& host, 
                                                  int port) {
        return vsdaManager->LeaderHeartbeat(leaderId, host, port);
    });
    
    // Cluster status endpoint
    RPCServer_->bind("GetClusterStatus", [vsdaManager]() { 
        return vsdaManager->GetClusterStatus().dump(); 
    });
    
    // Ping endpoint
    RPCServer_->bind("Ping", []() { return "PONG"; });
}


}; // Close Namespace API
}; // Close Namespace API
}; // Close Namespace BG