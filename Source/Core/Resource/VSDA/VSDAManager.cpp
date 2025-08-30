#include <Resource/VSDA/VSDAManager.h>
#include <rpc/client.h>
#include <algorithm>
#include <stdexcept>

namespace BG {
namespace API {
namespace VSDA {

Manager::Manager(BG::Common::Logger::LoggingSystem* _Logger, Config::Config* _Config, Server::Server* _Server) 
    : Logger_(_Logger), Config_(_Config), Server_(_Server), RequestThreadsExit_(false) {
    
    // Get configuration values
    NodeTimeoutSeconds_ = 5;//Config_->GetInt("VSDA.NodeTimeoutSeconds", 30);
    LeaderHeartbeatTimeoutSeconds_ = 2;//Config_->GetInt("VSDA.LeaderHeartbeatTimeoutSeconds", 10);
    
    Logger_->Log("Starting VSDA Manager", 1);
    
    // Start management threads
    NodeManagerThread_ = std::thread(&Manager::NodeManager, this);
    LeaderMonitorThread_ = std::thread(&Manager::LeaderMonitor, this);
    
    Logger_->Log("VSDA Manager started successfully", 1);
}

Manager::~Manager() {
    Logger_->Log("Shutting down VSDA Manager", 1);
    
    // Signal threads to exit
    RequestThreadsExit_ = true;
    
    // Join threads
    if (NodeManagerThread_.joinable()) {
        NodeManagerThread_.join();
    }
    
    if (LeaderMonitorThread_.joinable()) {
        LeaderMonitorThread_.join();
    }
    
    Logger_->Log("VSDA Manager shut down successfully", 1);
}

void Manager::NodeManager() {
    Logger_->Log("Started VSDA Node Manager thread", 2);
    
    while (!RequestThreadsExit_) {
        try {
            // Clean up stale nodes
            CleanupStaleNodes();
            
            // Sleep for a while before next check
            std::this_thread::sleep_for(std::chrono::seconds(5));
        } catch (const std::exception& e) {
            Logger_->Log("Error in NodeManager thread: " + std::string(e.what()), 4);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    
    Logger_->Log("Exiting VSDA Node Manager thread", 2);
}

void Manager::LeaderMonitor() {
    Logger_->Log("Started VSDA Leader Monitor thread", 2);
    
    while (!RequestThreadsExit_) {
        try {
            // Check if we have a leader
            if (HasLeader()) {
                // Check if leader is still responsive
                if (!CheckLeaderHealth()) {
                    Logger_->Log("Leader is not responsive, initiating election", 3);
                    InitiateElection();
                }
            } else {
                // No leader, try to elect one if we have nodes
                if (CountNodes() > 0) {
                    Logger_->Log("No leader available, initiating election", 3);
                    InitiateElection();
                }
            }
            
            // Sleep for a while before next check
            std::this_thread::sleep_for(std::chrono::seconds(2));
        } catch (const std::exception& e) {
            Logger_->Log("Error in LeaderMonitor thread: " + std::string(e.what()), 4);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    
    Logger_->Log("Exiting VSDA Leader Monitor thread", 2);
}

void Manager::CleanupStaleNodes() {
    std::lock_guard<std::mutex> nodesLock(NodesMutex_);
    time_t now = time(nullptr);
    
    std::vector<std::string> nodesToRemove;
    
    // Find stale nodes
    for (const auto& pair : Nodes_) {
        const auto& node = pair.second;
        if (now - node.last_seen > NodeTimeoutSeconds_) {
            nodesToRemove.push_back(node.id);
            Logger_->Log("Removing stale node: " + node.id, 3);
        }
    }
    
    // Remove stale nodes
    for (const auto& nodeId : nodesToRemove) {
        Nodes_.erase(nodeId);
        
        // If the removed node was the leader, clear leader
        if (HasLeader() && CurrentLeaderId_ == nodeId) {
            std::lock_guard<std::mutex> leaderLock(LeaderMutex_);
            CurrentLeaderId_.clear();
            Logger_->Log("Leader node removed due to timeout: " + nodeId, 2);
        }
    }
}

bool Manager::CheckLeaderHealth() {
    std::lock_guard<std::mutex> nodesLock(NodesMutex_);
    
    if (CurrentLeaderId_.empty()) {
        return false;
    }
    
    auto it = Nodes_.find(CurrentLeaderId_);
    if (it == Nodes_.end()) {
        return false;
    }
    
    const auto& leader = it->second;
    time_t now = time(nullptr);
    
    // Check if leader has checked in recently
    if (now - leader.last_seen > LeaderHeartbeatTimeoutSeconds_) {
        return false;
    }
    
    // Try to ping the leader directly
    try {
        ::rpc::client leaderClient(leader.host, leader.port);
        leaderClient.set_timeout(1000);
        auto response = leaderClient.call("Ping", "API Server").as<std::string>();
        return response == "PONG";
    } catch (const std::exception& e) {
        Logger_->Log("Leader health check failed: " + std::string(e.what()), 3);
        return false;
    }
}

void Manager::InitiateElection() {
    std::lock_guard<std::mutex> nodesLock(NodesMutex_);
    
    if (Nodes_.empty()) {
        Logger_->Log("No nodes available for election", 3);
        return;
    }
    
    // Simple election algorithm: choose the node with the lowest ID
    // In a real implementation, you might want a more sophisticated approach
    std::string newLeaderId;
    NodeInfo newLeader;
    
    for (const auto& pair : Nodes_) {
        if (newLeaderId.empty() || pair.first < newLeaderId) {
            newLeaderId = pair.first;
            newLeader = pair.second;
        }
    }
    
    if (!newLeaderId.empty()) {
        std::lock_guard<std::mutex> leaderLock(LeaderMutex_);
        CurrentLeaderId_ = newLeaderId;
        
        // Notify the new leader
        try {
            ::rpc::client leaderClient(newLeader.host, newLeader.port);
            leaderClient.set_timeout(2000);
            leaderClient.call("NotifyLeadershipChange", true);
            Logger_->Log("Elected new leader: " + newLeaderId, 2);
        } catch (const std::exception& e) {
            Logger_->Log("Failed to notify new leader: " + std::string(e.what()), 3);
        }
    }
}

nlohmann::json Manager::RegisterNode(const std::string& nodeId, const std::string& host, int port, bool isLeader) {
    Logger_->Log("Registering node: " + nodeId + " (" + host + ":" + std::to_string(port) + ")", 2);
    
    NodeInfo node;
    node.id = nodeId;
    node.host = host;
    node.port = port;
    node.is_leader = isLeader;
    node.last_seen = time(nullptr);
    
    {
        std::lock_guard<std::mutex> lock(NodesMutex_);
        Nodes_[nodeId] = node;
    }
    
    // If this node claims to be leader, verify and update
    if (isLeader) {
        std::lock_guard<std::mutex> leaderLock(LeaderMutex_);
        CurrentLeaderId_ = nodeId;
        Logger_->Log("Node registered as leader: " + nodeId, 2);
    }
    
    // Prepare response
    nlohmann::json response;
    response["status"] = "registered";
    
    if (HasLeader()) {
        auto leader = GetLeader();
        response["leader"] = leader.ToJSON();
    }
    
    return response;
}

nlohmann::json Manager::CheckIn(const std::string& nodeId, const std::string& host, int port, bool isLeader) {
    Logger_->Log("Node check-in: " + nodeId, 3);
    
    // Update node information
    {
        std::lock_guard<std::mutex> lock(NodesMutex_);
        auto it = Nodes_.find(nodeId);
        if (it != Nodes_.end()) {
            it->second.last_seen = time(nullptr);
            it->second.host = host;
            it->second.port = port;
        } else {
            // Node not registered, register it
            return RegisterNode(nodeId, host, port, isLeader);
        }
    }
    
    // If this node claims to be leader, verify and update
    if (isLeader) {
        std::lock_guard<std::mutex> leaderLock(LeaderMutex_);
        CurrentLeaderId_ = nodeId;
        Logger_->Log("Node check-in as leader: " + nodeId, 3);
    }
    
    // Prepare response
    nlohmann::json response;
    response["status"] = "checked_in";
    
    if (HasLeader()) {
        auto leader = GetLeader();
        response["leader"] = leader.ToJSON();
        
        // If this node is not the leader, tell it who is
        if (nodeId != CurrentLeaderId_) {
            response["is_follower"] = true;
        }
    } else if (isLeader) {
        // This node claims to be leader and we don't have one, accept it
        response["status"] = "elected";
        response["leader"]["id"] = nodeId;
        response["leader"]["host"] = host;
        response["leader"]["port"] = port;
    } else if (CountNodes() > 1) {
        // No leader but we have nodes, suggest election
        response["suggest_election"] = true;
    }
    
    return response;
}

nlohmann::json Manager::ElectLeader(const std::string& nodeId, const std::string& host, int port) {
    Logger_->Log("Leader election requested by: " + nodeId, 2);
    
    // Update node information
    CheckIn(nodeId, host, port, false);
    
    nlohmann::json response;
    
    if (HasLeader()) {
        // We already have a leader
        auto leader = GetLeader();
        
        // Check if the current leader is responsive
        if (CheckLeaderHealth()) {
            response["status"] = "leader_exists";
            response["leader"] = leader.ToJSON();
            return response;
        } else {
            Logger_->Log("Current leader is not responsive, proceeding with election", 3);
        }
    }
    
    // No leader or leader is not responsive, proceed with election
    // For simplicity, we'll elect the requesting node
    // In a real implementation, you might want a more sophisticated election algorithm
    
    std::lock_guard<std::mutex> leaderLock(LeaderMutex_);
    CurrentLeaderId_ = nodeId;
    
    response["status"] = "elected";
    response["leader"]["id"] = nodeId;
    response["leader"]["host"] = host;
    response["leader"]["port"] = port;
    
    Logger_->Log("Elected new leader: " + nodeId, 2);
    
    return response;
}

bool Manager::LeaderHeartbeat(const std::string& leaderId, const std::string& host, int port) {
    Logger_->Log("Leader heartbeat: " + leaderId, 4);
    
    // Verify this is indeed the current leader
    if (HasLeader() && CurrentLeaderId_ == leaderId) {
        // Update node information
        CheckIn(leaderId, host, port, true);
        return true;
    }
    
    // This node claims to be leader but we don't recognize it as such
    Logger_->Log("Unauthorized leader heartbeat: " + leaderId, 3);
    return false;
}

nlohmann::json Manager::GetFollowers() {
    std::lock_guard<std::mutex> nodesLock(NodesMutex_);
    
    nlohmann::json followers = nlohmann::json::array();
    
    for (const auto& pair : Nodes_) {
        const auto& node = pair.second;
        if (!node.is_leader && node.id != CurrentLeaderId_) {
            followers.push_back(node.ToJSON());
        }
    }
    
    return followers;
}

nlohmann::json Manager::GetNodes() {
    std::lock_guard<std::mutex> nodesLock(NodesMutex_);
    
    nlohmann::json nodes = nlohmann::json::array();
    
    for (const auto& pair : Nodes_) {
        nodes.push_back(pair.second.ToJSON());
    }
    
    return nodes;
}

bool Manager::ClusterStatus(const nlohmann::json& status) {
    if (!HasLeader()) {
        Logger_->Log("Cluster status received but no leader set", 3);
        return false;
    }
    
    // Store or process cluster status
    // This could be stored in a database or used for monitoring
    
    Logger_->Log("Cluster status update received from leader", 3);
    return true;
}

bool Manager::LeaderPromoted(const std::string& nodeId, const std::string& host, int port) {
    Logger_->Log("Leader promoted: " + nodeId, 2);
    
    // Verify this node exists
    std::lock_guard<std::mutex> nodesLock(NodesMutex_);
    auto it = Nodes_.find(nodeId);
    if (it == Nodes_.end()) {
        Logger_->Log("Leader promotion for unknown node: " + nodeId, 3);
        return false;
    }
    
    // Update leader information
    std::lock_guard<std::mutex> leaderLock(LeaderMutex_);
    CurrentLeaderId_ = nodeId;
    
    // Update node information
    it->second.is_leader = true;
    it->second.last_seen = time(nullptr);
    
    return true;
}

bool Manager::LeaderDemoted(const std::string& nodeId) {
    Logger_->Log("Leader demoted: " + nodeId, 2);
    
    // Verify this node is the current leader
    if (!HasLeader() || CurrentLeaderId_ != nodeId) {
        Logger_->Log("Leader demotion for non-leader node: " + nodeId, 3);
        return false;
    }
    
    // Update node information
    std::lock_guard<std::mutex> nodesLock(NodesMutex_);
    auto it = Nodes_.find(nodeId);
    if (it != Nodes_.end()) {
        it->second.is_leader = false;
    }
    
    // Clear leader information
    std::lock_guard<std::mutex> leaderLock(LeaderMutex_);
    CurrentLeaderId_.clear();
    
    return true;
}

bool Manager::NodeHealth(const nlohmann::json& healthData) {
    if (!healthData.contains("node_id")) {
        Logger_->Log("Node health report missing node_id", 3);
        return false;
    }
    
    std::string nodeId = healthData["node_id"];
    Logger_->Log("Node health report: " + nodeId, 4);
    
    // Update node last seen
    std::lock_guard<std::mutex> nodesLock(NodesMutex_);
    auto it = Nodes_.find(nodeId);
    if (it != Nodes_.end()) {
        it->second.last_seen = time(nullptr);
    }
    
    // Store or process health data
    // This could be stored in a database or used for monitoring
    
    return true;
}

void Manager::RegisterMessageCallback(const std::string& path, MessageCallback callback) {
    std::lock_guard<std::mutex> lock(MessageCallbacksMutex_);
    MessageCallbacks_[path] = callback;
    Logger_->Log("Registered message callback for path: " + path, 2);
}

std::string Manager::RouteMessage(const std::string& path, const std::string& data) {
    std::lock_guard<std::mutex> lock(MessageCallbacksMutex_);
    auto it = MessageCallbacks_.find(path);
    if (it != MessageCallbacks_.end()) {
        return it->second(data);
    }
    
    Logger_->Log("No callback registered for path: " + path, 4);
    throw std::runtime_error("No callback registered for path: " + path);
}

nlohmann::json Manager::GetClusterStatus() {
    nlohmann::json status;
    
    status["node_count"] = CountNodes();
    status["has_leader"] = HasLeader();
    
    if (HasLeader()) {
        status["leader"] = GetLeader().ToJSON();
    }
    
    status["nodes"] = GetNodes();
    status["followers"] = GetFollowers();
    
    return status;
}

NodeInfo Manager::GetLeader() const {
    std::lock_guard<std::mutex> leaderLock(const_cast<std::mutex&>(LeaderMutex_));
    
    if (CurrentLeaderId_.empty()) {
        return NodeInfo();
    }
    
    std::lock_guard<std::mutex> nodesLock(const_cast<std::mutex&>(NodesMutex_));
    auto it = Nodes_.find(CurrentLeaderId_);
    if (it != Nodes_.end()) {
        return it->second;
    }
    
    return NodeInfo();
}

bool Manager::HasLeader() const {
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(LeaderMutex_));
    return !CurrentLeaderId_.empty();
}

size_t Manager::CountNodes() const {
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(NodesMutex_));
    return Nodes_.size();
}
}; // namespace VSDA
}; // namespace API
}; // namespace BG