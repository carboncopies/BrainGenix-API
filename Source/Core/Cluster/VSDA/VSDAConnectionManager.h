#pragma once

#include <RPC/RPCManager.h>
#include <Cluster/BidirectionalRPC.h>
#include <BG/Common/Logger/Logger.h>
#include <memory>
#include <mutex>
#include <string>

namespace BG {
namespace API {
namespace API {

class VSDAConnectionManager {
public:
    VSDAConnectionManager(BG::Common::Logger::LoggingSystem* logger, 
                        RPCManager* rpcManager);
    ~VSDAConnectionManager();

    void Initialize();
    void RegisterVSDANode(const std::string& nodeId, const std::string& host, int port);
    void SetVSDALeader(const std::string& nodeId);
    std::string CallVSDALeader(const std::string& method, const std::string& params = "");
    bool HasVSDALeader() const;

private:
    BG::Common::Logger::LoggingSystem* logger_;
    RPCManager* rpcManager_;
    std::unique_ptr<BidirectionalRpc> leaderRpc_;
    mutable std::mutex mutex_;
};

} // namespace API
} // namespace API
} // namespace BG