#pragma once

#include <RPC/RPCManager.h>
#include <Cluster/BidirectionalRPC.h>
#include <BG/Common/Logger/Logger.h>
#include <Config/ConfigParser.h>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <functional>
#include <nlohmann/json.hpp>

class RPCManager;

class VSDAConnectionManager
{
public:
    VSDAConnectionManager(BG::Common::Logger::LoggingSystem *logger,
                          RPCManager *rpcManager,
                          ConfigParser *config);
    ~VSDAConnectionManager();

    void Initialize();
    std::string RegisterVSDANode(const std::string &jsonRequest);
    std::string SetVSDALeader(const std::string &jsonRequest);
    std::string CallVSDALeader(const std::string &method, const std::string &params = "");
    bool HasVSDALeader() const;

    // BidirectionalRpc* GetBidirectionalRPC();

    /**
    Helper function that will register a callback to the bidirectionalrpc class whenever it has been created or re-created.
    Will save the callback for actual binding once bidirectional leader rpc is ready.
    */
    bool RegisterBidirectionalCallback(std::string _CallbackName, std::function<std::string(const std::string &)> _callback);

private:
    BG::Common::Logger::LoggingSystem *logger_;
    RPCManager *rpcManager_;
    ConfigParser *config_;
    std::unique_ptr<BidirectionalRpc> leaderRpc_;
    mutable std::mutex mutex_;
    std::string currentLeaderNodeId_;

    // Storage for pending callbacks
    std::vector<std::pair<std::string, std::function<std::string(const std::string &)>>> pendingCallbacks_;
};
