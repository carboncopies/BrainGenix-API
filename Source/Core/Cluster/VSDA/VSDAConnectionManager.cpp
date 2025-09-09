#include "VSDAConnectionManager.h"
#include <stdexcept>
#include <nlohmann/json.hpp>



using json = nlohmann::json;

VSDAConnectionManager::VSDAConnectionManager(BG::Common::Logger::LoggingSystem* logger, 
                                           RPCManager* rpcManager)
    : logger_(logger), rpcManager_(rpcManager) {}

VSDAConnectionManager::~VSDAConnectionManager() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (leaderRpc_) {
        leaderRpc_->Stop();
    }
}

void VSDAConnectionManager::Initialize() {
    // Register the endpoints that VSDA nodes will call to register themselves
    rpcManager_->AddRoute("RegisterVSDANode", logger_, 
        [this](const std::string& jsonRequest) {
            return this->RegisterVSDANode(jsonRequest);
        });
    
    rpcManager_->AddRoute("SetVSDALeader", logger_, 
        [this](const std::string& jsonRequest) {
            return this->SetVSDALeader(jsonRequest);
        });
    
    logger_->Log("[VSDAConnectionManager] Initialized VSDA RPC endpoints", 4);
}

std::string VSDAConnectionManager::RegisterVSDANode(const std::string& jsonRequest) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    try {
        json request = json::parse(jsonRequest);
        std::string nodeId = request["nodeId"];
        std::string host = request["host"];
        int port = request["port"];
        
        logger_->Log("[VSDAConnectionManager] Received registration from VSDA node: " + 
                    nodeId + " at " + host + ":" + std::to_string(port), 4);
        
        // Create a bidirectional RPC connection to the VSDA node
        if (!leaderRpc_) {
            leaderRpc_ = std::make_unique<BidirectionalRpc>(8003, true, logger_, 5000, "VSDALeader");
            leaderRpc_->SetAdvertisedHost("localhost");
            leaderRpc_->Start();
            logger_->Log("[VSDAConnectionManager] Created bidirectional RPC client", 4);
        }
        
        // Update the connection to point to the VSDA node
        leaderRpc_->UpdatePeer(host, port);
        logger_->Log("[VSDAConnectionManager] Updated peer to: " + host + ":" + std::to_string(port), 4);
        
        // Test the connection
        bool healthCheck = leaderRpc_->Call<bool>("HealthCheck");
        if (!healthCheck) {
            throw std::runtime_error("Health check failed");
        }
        
        logger_->Log("[VSDAConnectionManager] Successfully connected to VSDA node: " + nodeId, 4);
        
        // Return success response
        json response;
        response["success"] = true;
        response["message"] = "Node registered successfully";
        return response.dump();
        
    } catch (const std::exception& e) {
        logger_->Log("[VSDAConnectionManager] Failed to register VSDA node: " + std::string(e.what()), 8);
        if (leaderRpc_) {
            leaderRpc_->Stop();
            leaderRpc_.reset();
        }
        
        // Return error response
        json response;
        response["success"] = false;
        response["error"] = e.what();
        return response.dump();
    }
}

std::string VSDAConnectionManager::SetVSDALeader(const std::string& jsonRequest) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    try {
        json request = json::parse(jsonRequest);
        std::string nodeId = request["nodeId"];
        
        logger_->Log("[VSDAConnectionManager] Set VSDA leader: " + nodeId, 4);
        currentLeaderNodeId_ = nodeId;
        
        // Return success response
        json response;
        response["success"] = true;
        response["message"] = "Leader set successfully";
        return response.dump();
        
    } catch (const std::exception& e) {
        logger_->Log("[VSDAConnectionManager] Failed to set VSDA leader: " + std::string(e.what()), 8);
        
        // Return error response
        json response;
        response["success"] = false;
        response["error"] = e.what();
        return response.dump();
    }
}

std::string VSDAConnectionManager::CallVSDALeader(const std::string& method, 
                                                const std::string& params) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!leaderRpc_) {
        throw std::runtime_error("No VSDA leader configured");
    }
    
    try {
        if (params.empty()) {
            return leaderRpc_->Call<std::string>(method);
        } else {
            return leaderRpc_->Call<std::string>(method, params);
        }
    } catch (const std::exception& e) {
        logger_->Log("[VSDAConnectionManager] RPC call to VSDA leader failed: " + 
                    std::string(e.what()), 8);
        throw;
    }
}

bool VSDAConnectionManager::HasVSDALeader() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return leaderRpc_ && leaderRpc_->IsConnected();
}

