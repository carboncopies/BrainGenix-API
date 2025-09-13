#pragma once

#include <RPC/RPCManager.h>
#include <Cluster/BidirectionalRPC.h>
#include <BG/Common/Logger/Logger.h>
#include <Config/ConfigParser.h>
#include <memory>
#include <mutex>
#include <string>
#include <nlohmann/json.hpp>


class RPCManager;

class VSDAConnectionManager {
public:
    VSDAConnectionManager(BG::Common::Logger::LoggingSystem* logger, 
                        RPCManager* rpcManager,
                        ConfigParser* config);
    ~VSDAConnectionManager();

    void Initialize();
    std::string RegisterVSDANode(const std::string& jsonRequest);
    std::string SetVSDALeader(const std::string& jsonRequest);
    std::string CallVSDALeader(const std::string& method, const std::string& params = "");
    bool HasVSDALeader() const;

private:
    BG::Common::Logger::LoggingSystem* logger_;
    RPCManager* rpcManager_;
    ConfigParser* config_;
    std::unique_ptr<BidirectionalRpc> leaderRpc_;
    mutable std::mutex mutex_;
    std::string currentLeaderNodeId_;
};

