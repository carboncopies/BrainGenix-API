#include "VSDAConnectionManager.h"
#include <rpc/client.h>
#include <stdexcept>

namespace BG {
namespace API {
namespace API {

VSDAConnectionManager::VSDAConnectionManager(BG::Common::Logger::LoggingSystem* logger, 
                                           RPCManager* rpcManager)
    : logger_(logger), rpcManager_(rpcManager) {}

VSDAConnectionManager::~VSDAConnectionManager() {
    std::lock_guard<std::mutex> lock(mutex_);
    vsdaNodes_.clear();
}

void VSDAConnectionManager::Initialize() {
    // Register the endpoints that VSDA nodes will call to register themselves
    rpcManager_->AddRoute("RegisterVSDANode", logger_, 
        [this](const std::string& nodeId, const std::string& host, int port) {
            this->RegisterVSDANode(nodeId, host, port);
            return true;
        });
    
    rpcManager_->AddRoute("SetVSDALeader", logger_, 
        [this](const std::string& nodeId) {
            this->SetVSDALeader(nodeId);
            return true;
        });
    
    logger_->Log("[VSDAConnectionManager] Initialized VSDA RPC endpoints", 4);
}

void VSDAConnectionManager::RegisterVSDANode(const std::string& nodeId, 
                                           const std::string& host, int port) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    try {
        // Create a client connection back to the VSDA node
        auto client = std::make_shared<rpc::client>(host, port);
        client->set_timeout(5000);
        
        // Test the connection with a simple health check
        client->call("HealthCheck");
        
        vsdaNodes_[nodeId] = {host, port, client};
        
        logger_->Log("[VSDAConnectionManager] Registered VSDA node: " + nodeId + 
                    " at " + host + ":" + std::to_string(port), 4);
    } catch (const std::exception& e) {
        logger_->Log("[VSDAConnectionManager] Failed to register VSDA node " + 
                    nodeId + ": " + e.what(), 8);
        throw;
    }
}

void VSDAConnectionManager::UnregisterVSDANode(const std::string& nodeId) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (vsdaNodes_.find(nodeId) != vsdaNodes_.end()) {
        vsdaNodes_.erase(nodeId);
        
        // If the unregistered node was the leader, clear the leader reference
        if (vsdaLeaderNodeId_ == nodeId) {
            vsdaLeaderNodeId_.clear();
            logger_->Log("[VSDAConnectionManager] VSDA leader disconnected: " + nodeId, 5);
        }
        
        logger_->Log("[VSDAConnectionManager] Unregistered VSDA node: " + nodeId, 4);
    }
}

void VSDAConnectionManager::SetVSDALeader(const std::string& nodeId) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (vsdaNodes_.find(nodeId) == vsdaNodes_.end()) {
        logger_->Log("[VSDAConnectionManager] Attempt to set unknown node as VSDA leader: " + 
                    nodeId, 8);
        throw std::runtime_error("Unknown VSDA node: " + nodeId);
    }
    
    vsdaLeaderNodeId_ = nodeId;
    logger_->Log("[VSDAConnectionManager] Set VSDA leader: " + nodeId, 4);
}

std::string VSDAConnectionManager::CallVSDALeader(const std::string& method, 
                                                const std::string& params) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (vsdaLeaderNodeId_.empty()) {
        throw std::runtime_error("No VSDA leader available");
    }
    
    return CallVSDANode(vsdaLeaderNodeId_, method, params);
}

std::string VSDAConnectionManager::CallVSDANode(const std::string& nodeId, 
                                              const std::string& method, 
                                              const std::string& params) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (vsdaNodes_.find(nodeId) == vsdaNodes_.end()) {
        throw std::runtime_error("VSDA node not connected: " + nodeId);
    }
    
    try {
        auto& node = vsdaNodes_[nodeId];
        if (params.empty()) {
            return node.client->call(method).as<std::string>();
        } else {
            return node.client->call(method, params).as<std::string>();
        }
    } catch (const std::exception& e) {
        logger_->Log("[VSDAConnectionManager] RPC call to VSDA node " + nodeId + " failed: " + 
                    std::string(e.what()), 8);
        throw;
    }
}

bool VSDAConnectionManager::HasVSDALeader() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return !vsdaLeaderNodeId_.empty() && vsdaNodes_.find(vsdaLeaderNodeId_) != vsdaNodes_.end();
}

} // namespace API
} // namespace API
} // namespace BG