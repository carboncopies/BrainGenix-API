#include "VSDAConnectionManager.h"
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <rpc/client.h>



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
    std::string nodeId;
    std::string host;
    int port = 0;

    try {
        json request = json::parse(jsonRequest);

        if (!request.is_object()) {
            throw std::runtime_error("Registration request must be a JSON object");
        }
        if (!request.contains("nodeId") || !request["nodeId"].is_string()) {
            throw std::runtime_error("Registration request requires string nodeId");
        }
        if (!request.contains("host") || !request["host"].is_string()) {
            throw std::runtime_error("Registration request requires string host");
        }
        if (!request.contains("port") || !request["port"].is_number_integer()) {
            throw std::runtime_error("Registration request requires integer port");
        }

        nodeId = request["nodeId"].get<std::string>();
        host = request["host"].get<std::string>();
        port = request["port"].get<int>();
        if (nodeId.empty()) {
            throw std::runtime_error("Registration request nodeId cannot be empty");
        }
        if (host.empty()) {
            throw std::runtime_error("Registration request host cannot be empty");
        }
        if (port <= 0 || port > 65535) {
            throw std::runtime_error("Registration request port must be between 1 and 65535");
        }
    } catch (const std::exception& e) {
        logger_->Log("[VSDAConnectionManager] Rejected invalid VSDA node registration: " + std::string(e.what()), 8);

        json response;
        response["success"] = false;
        response["error"] = e.what();
        return response.dump();
    }

    try {
        logger_->Log("[VSDAConnectionManager] Received registration from VSDA node: " + 
                    nodeId + " at " + host + ":" + std::to_string(port), 4);

        rpc::client healthClient(host, port);
        healthClient.set_timeout(5000);
        bool healthCheck = healthClient.call("HealthCheck").as<bool>();
        if (!healthCheck) {
            throw std::runtime_error("Health check failed");
        }

        std::lock_guard<std::mutex> lock(mutex_);
        bool createdLeaderRpc = false;
        const bool hadPreviousLeader = leaderRpc_ && !currentLeaderHost_.empty() && currentLeaderPort_ > 0;
        const std::string previousLeaderHost = currentLeaderHost_;
        const int previousLeaderPort = currentLeaderPort_;

        // Create a bidirectional RPC connection to the VSDA node
        if (!leaderRpc_) {
            leaderRpc_ = std::make_unique<BidirectionalRpc>(8003, true, logger_, 5000, "VSDALeader");
            leaderRpc_->SetAdvertisedHost("localhost");
            leaderRpc_->Start();
            createdLeaderRpc = true;
            logger_->Log("[VSDAConnectionManager] Created bidirectional RPC client", 4);
        }

        // Update the connection to point to the VSDA node
        try {
            leaderRpc_->UpdatePeer(host, port);
            bool leaderHealthCheck = leaderRpc_->Call<bool>("HealthCheck");
            if (!leaderHealthCheck) {
                throw std::runtime_error("Bidirectional health check failed");
            }
        } catch (...) {
            if (createdLeaderRpc && leaderRpc_) {
                leaderRpc_->Stop();
                leaderRpc_.reset();
            } else if (hadPreviousLeader && leaderRpc_) {
                leaderRpc_->UpdatePeer(previousLeaderHost, previousLeaderPort);
            }
            throw;
        }
        logger_->Log("[VSDAConnectionManager] Updated peer to: " + host + ":" + std::to_string(port), 4);

        logger_->Log("[VSDAConnectionManager] Successfully connected to VSDA node: " + nodeId, 4);
        currentLeaderNodeId_ = nodeId;
        currentLeaderHost_ = host;
        currentLeaderPort_ = port;
        
        // Return success response
        json response;
        response["success"] = true;
        response["message"] = "Node registered successfully";
        return response.dump();
        
    } catch (const std::exception& e) {
        logger_->Log("[VSDAConnectionManager] Failed to register VSDA node: " + std::string(e.what()), 8);
        
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
