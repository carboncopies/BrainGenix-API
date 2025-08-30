//==============================================================//
// BrainGenix-VSDA API Server Integration                       //
//==============================================================//

#pragma once

// Standard Libraries
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <functional>

// Third-Party Libraries
#include <rpc/server.h>
#include <nlohmann/json.hpp>

// Internal Libraries
#include <BG/Common/Logger/Logger.h>
#include <Config/Config.h>
#include <Server/Server.h>

namespace BG {
namespace API {
namespace VSDA {

// Node information structure
struct NodeInfo {
    std::string id;
    std::string host;
    int port;
    bool is_leader;
    time_t last_seen;
    
    bool operator<(const NodeInfo& other) const {
        return id < other.id;
    }
    
    nlohmann::json ToJSON() const {
        nlohmann::json j;
        j["id"] = id;
        j["host"] = host;
        j["port"] = port;
        j["is_leader"] = is_leader;
        j["last_seen"] = last_seen;
        return j;
    }
    
    static NodeInfo FromJSON(const nlohmann::json& j) {
        NodeInfo node;
        node.id = j.value("id", "");
        node.host = j.value("host", "");
        node.port = j.value("port", 0);
        node.is_leader = j.value("is_leader", false);
        node.last_seen = j.value("last_seen", time(nullptr));
        return node;
    }
};

// Message callback type
using MessageCallback = std::function<std::string(const std::string&)>;

/**
 * @brief Manages VSDA nodes, leader election, and message routing
 */
class Manager {
private:
    Config::Config* Config_; /**< Pointer to the configuration instance */
    Server::Server* Server_; /**< Pointer to server struct */
    BG::Common::Logger::LoggingSystem* Logger_; /**< Pointer to logging system */

    std::atomic<bool> RequestThreadsExit_; /**< Signal to exit management threads */
    std::thread NodeManagerThread_; /**< Thread for node management tasks */
    std::thread LeaderMonitorThread_; /**< Thread for monitoring leader health */

    std::mutex NodesMutex_; /**< Mutex for nodes access */
    std::map<std::string, NodeInfo> Nodes_; /**< Map of all registered nodes */
    
    std::mutex LeaderMutex_; /**< Mutex for leader access */
    std::string CurrentLeaderId_; /**< ID of the current leader */
    
    std::mutex MessageCallbacksMutex_; /**< Mutex for message callbacks */
    std::map<std::string, MessageCallback> MessageCallbacks_; /**< Registered message callbacks */

    int NodeTimeoutSeconds_; /**< Timeout for node cleanup */
    int LeaderHeartbeatTimeoutSeconds_; /**< Timeout for leader heartbeat */

    /**
     * @brief Manages node lifecycle in a separate thread
     */
    void NodeManager();

    /**
     * @brief Monitors leader health in a separate thread
     */
    void LeaderMonitor();

    /**
     * @brief Cleans up stale nodes
     */
    void CleanupStaleNodes();

    /**
     * @brief Checks if leader is still responsive
     */
    bool CheckLeaderHealth();

    /**
     * @brief Initiates a new leader election
     */
    void InitiateElection();

public:
    /**
     * @brief Constructs a VSDA manager
     * 
     * @param _Logger Pointer to the logging system instance
     * @param _Config Pointer to the configuration instance
     * @param _Server Pointer to the server instance
     */
    Manager(BG::Common::Logger::LoggingSystem* _Logger, Config::Config* _Config, Server::Server* _Server);

    /**
     * @brief Destroys the Manager object
     */
    ~Manager();

    /**
     * @brief Registers a node with the VSDA cluster
     * 
     * @param nodeId ID of the node
     * @param host Host address of the node
     * @param port Port of the node
     * @param isLeader Whether the node claims to be leader
     * @return nlohmann::json Response with cluster status
     */
    nlohmann::json RegisterNode(const std::string& nodeId, const std::string& host, int port, bool isLeader = false);

    /**
     * @brief Handles node check-in
     * 
     * @param nodeId ID of the node
     * @param host Host address of the node
     * @param port Port of the node
     * @param isLeader Whether the node claims to be leader
     * @return nlohmann::json Response with cluster status
     */
    nlohmann::json CheckIn(const std::string& nodeId, const std::string& host, int port, bool isLeader = false);

    /**
     * @brief Initiates a leader election
     * 
     * @param nodeId ID of the node requesting election
     * @param host Host address of the node
     * @param port Port of the node
     * @return nlohmann::json Election result
     */
    nlohmann::json ElectLeader(const std::string& nodeId, const std::string& host, int port);

    /**
     * @brief Handles leader heartbeat
     * 
     * @param leaderId ID of the leader node
     * @param host Host address of the leader
     * @param port Port of the leader
     * @return bool True if heartbeat was accepted
     */
    bool LeaderHeartbeat(const std::string& leaderId, const std::string& host, int port);

    /**
     * @brief Gets the list of followers
     * 
     * @return nlohmann::json List of follower nodes
     */
    nlohmann::json GetFollowers();

    /**
     * @brief Gets all nodes in the cluster
     * 
     * @return nlohmann::json List of all nodes
     */
    nlohmann::json GetNodes();

    /**
     * @brief Handles cluster status report from leader
     * 
     * @param status Cluster status data
     * @return bool True if status was accepted
     */
    bool ClusterStatus(const nlohmann::json& status);

    /**
     * @brief Handles leader promotion notification
     * 
     * @param nodeId ID of the promoted node
     * @param host Host address of the promoted node
     * @param port Port of the promoted node
     * @return bool True if notification was accepted
     */
    bool LeaderPromoted(const std::string& nodeId, const std::string& host, int port);

    /**
     * @brief Handles leader demotion notification
     * 
     * @param nodeId ID of the demoted node
     * @return bool True if notification was accepted
     */
    bool LeaderDemoted(const std::string& nodeId);

    /**
     * @brief Handles node health report
     * 
     * @param healthData Node health data
     * @return bool True if report was accepted
     */
    bool NodeHealth(const nlohmann::json& healthData);

    /**
     * @brief Registers a message callback for a specific path
     * 
     * @param path Message path
     * @param callback Callback function
     */
    void RegisterMessageCallback(const std::string& path, MessageCallback callback);

    /**
     * @routes a message to the appropriate handler
     * 
     * @param path Message path
     * @param data Message data
     * @return std::string Response
     */
    std::string RouteMessage(const std::string& path, const std::string& data);

    /**
     * @brief Gets the current cluster status
     * 
     * @return nlohmann::json Cluster status
     */
    nlohmann::json GetClusterStatus();

    /**
     * @brief Gets the current leader information
     * 
     * @return NodeInfo Leader node information
     */
    NodeInfo GetLeader() const;

    /**
     * @brief Checks if there is a current leader
     * 
     * @return bool True if there is a leader
     */
    bool HasLeader() const;

    /**
     * @brief Gets the number of active nodes
     * 
     * @return size_t Number of active nodes
     */
    size_t CountNodes() const;
};

}; // namespace VSDA
}; // namespace API
}; // namespace BG