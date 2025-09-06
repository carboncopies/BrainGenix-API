#include "VSDAConnectionManager.h"
#include <stdexcept>

namespace BG {
namespace API {
namespace API {

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
        logger_->Log("[VSDAConnectionManager] Received registration from VSDA node: " + 
                    nodeId + " at " + host + ":" + std::to_string(port), 4);
        
        // Create a bidirectional RPC connection to the VSDA node
        if (!leaderRpc_) {
            leaderRpc_ = std::make_unique<BidirectionalRpc>(0, true, logger_, 5000);
            leaderRpc_->SetAdvertisedHost("localhost");  // API server's hostname
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
        
        logger_->Log("[VSDAConnectionManager] Successfully connected to VSDA node: " + 
                    nodeId, 4);
        
    } catch (const std::exception& e) {
        logger_->Log("[VSDAConnectionManager] Failed to connect to VSDA node " + 
                    nodeId + ": " + e.what(), 8);
        if (leaderRpc_) {
            leaderRpc_->Stop();
            leaderRpc_.reset();
        }
        throw;
    }
}

void VSDAConnectionManager::SetVSDALeader(const std::string& nodeId) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    logger_->Log("[VSDAConnectionManager] Set VSDA leader: " + nodeId, 4);
    // For now, just log it - we might want to do more validation later
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

} // namespace API
} // namespace API
} // namespace BG