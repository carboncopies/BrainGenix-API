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


void RPCManager::BindVSDAEndpoints(BG::API::VSDA::Manager* VSDAManager) {
    if (!VSDAManager) return;
    
    // Bind VSDA RPC endpoints
    RPCServer_.get()->bind("VSDA_CheckIn", [VSDAManager](const std::string& checkInData) {
        try {
            nlohmann::json data = nlohmann::json::parse(checkInData);
            auto response = VSDAManager->CheckIn(
                data["node_id"].get<std::string>(),
                data["node_host"].get<std::string>(),
                data["node_port"].get<int>(),
                data.value("is_leader", false)
            );
            return response.dump();
        } catch (const std::exception& e) {
            nlohmann::json error;
            error["error"] = std::string("CheckIn failed: ") + e.what();
            return error.dump();
        }
    });
    
    // Bind all other VSDA methods similarly...
    RPCServer_.get()->bind("VSDA_ElectLeader", [VSDAManager](const std::string& electionData) {
        try {
            nlohmann::json data = nlohmann::json::parse(electionData);
            auto response = VSDAManager->ElectLeader(
                data["node_id"].get<std::string>(),
                data["node_host"].get<std::string>(),
                data["node_port"].get<int>()
            );
            return response.dump();
        } catch (const std::exception& e) {
            nlohmann::json error;
            error["error"] = std::string("ElectLeader failed: ") + e.what();
            return error.dump();
        }
    });
    
    RPCServer_.get()->bind("VSDA_LeaderHeartbeat", [VSDAManager](const std::string& heartbeatData) {
        try {
            nlohmann::json data = nlohmann::json::parse(heartbeatData);
            bool success = VSDAManager->LeaderHeartbeat(
                data["leader_id"].get<std::string>(),
                data["leader_host"].get<std::string>(),
                data["leader_port"].get<int>()
            );
            nlohmann::json response;
            response["success"] = success;
            return response.dump();
        } catch (const std::exception& e) {
            nlohmann::json error;
            error["error"] = std::string("LeaderHeartbeat failed: ") + e.what();
            return error.dump();
        }
    });
    
    RPCServer_.get()->bind("VSDA_GetFollowers", [VSDAManager]() {
        return VSDAManager->GetFollowers().dump();
    });
    
    RPCServer_.get()->bind("VSDA_GetNodes", [VSDAManager]() {
        return VSDAManager->GetNodes().dump();
    });
    
    RPCServer_.get()->bind("VSDA_ClusterStatus", [VSDAManager](const std::string& statusData) {
        try {
            nlohmann::json data = nlohmann::json::parse(statusData);
            bool success = VSDAManager->ClusterStatus(data);
            nlohmann::json response;
            response["success"] = success;
            return response.dump();
        } catch (const std::exception& e) {
            nlohmann::json error;
            error["error"] = std::string("ClusterStatus failed: ") + e.what();
            return error.dump();
        }
    });
    
    RPCServer_.get()->bind("VSDA_LeaderPromoted", [VSDAManager](const std::string& promotionData) {
        try {
            nlohmann::json data = nlohmann::json::parse(promotionData);
            bool success = VSDAManager->LeaderPromoted(
                data["node_id"].get<std::string>(),
                data["node_host"].get<std::string>(),
                data["node_port"].get<int>()
            );
            nlohmann::json response;
            response["success"] = success;
            return response.dump();
        } catch (const std::exception& e) {
            nlohmann::json error;
            error["error"] = std::string("LeaderPromoted failed: ") + e.what();
            return error.dump();
        }
    });
    
    RPCServer_.get()->bind("VSDA_LeaderDemoted", [VSDAManager](const std::string& demotionData) {
        try {
            nlohmann::json data = nlohmann::json::parse(demotionData);
            bool success = VSDAManager->LeaderDemoted(data["node_id"].get<std::string>());
            nlohmann::json response;
            response["success"] = success;
            return response.dump();
        } catch (const std::exception& e) {
            nlohmann::json error;
            error["error"] = std::string("LeaderDemoted failed: ") + e.what();
            return error.dump();
        }
    });
    
    RPCServer_.get()->bind("VSDA_NodeHealth", [VSDAManager](const std::string& healthData) {
        try {
            nlohmann::json data = nlohmann::json::parse(healthData);
            bool success = VSDAManager->NodeHealth(data);
            nlohmann::json response;
            response["success"] = success;
            return response.dump();
        } catch (const std::exception& e) {
            nlohmann::json error;
            error["error"] = std::string("NodeHealth failed: ") + e.what();
            return error.dump();
        }
    });
}


}; // Close Namespace API
}; // Close Namespace API
}; // Close Namespace BG