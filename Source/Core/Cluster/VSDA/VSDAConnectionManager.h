#pragma once

#include <RPC/RPCManager.h>
#include <BG/Common/Logger/Logger.h>
#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

namespace BG {
namespace API {
namespace API {

/**
 * @class VSDAConnectionManager
 * @brief Manages RPC connections to VSDA cluster nodes and tracks the current leader
 */
class VSDAConnectionManager {
public:
    VSDAConnectionManager(BG::Common::Logger::LoggingSystem* logger, 
                        RPCManager* rpcManager);
    ~VSDAConnectionManager();

    /**
     * @brief Initializes the VSDA connection manager with RPC endpoints
     */
    void Initialize();

    /**
     * @brief Registers a VSDA node that has connected to this API server
     */
    void RegisterVSDANode(const std::string& nodeId, const std::string& host, int port);

    /**
     * @brief Unregisters a VSDA node that has disconnected
     */
    void UnregisterVSDANode(const std::string& nodeId);

    /**
     * @brief Sets the current VSDA cluster leader
     */
    void SetVSDALeader(const std::string& nodeId);

    /**
     * @brief Makes an RPC call to the VSDA leader
     */
    std::string CallVSDALeader(const std::string& method, const std::string& params = "");

    /**
     * @brief Makes an RPC call to a specific VSDA node
     */
    std::string CallVSDANode(const std::string& nodeId, const std::string& method, 
                           const std::string& params = "");

    /**
     * @brief Checks if a VSDA leader is currently available
     */
    bool HasVSDALeader() const;

private:
    struct VSDANode {
        std::string host;
        int port;
        std::shared_ptr<rpc::client> client;
    };

    BG::Common::Logger::LoggingSystem* logger_;
    RPCManager* rpcManager_;
    std::unordered_map<std::string, VSDANode> vsdaNodes_;
    std::string vsdaLeaderNodeId_;
    mutable std::mutex mutex_;
};

} // namespace API
} // namespace API
} // namespace BG