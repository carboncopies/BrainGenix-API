#include "ClusterRPC.h"
#include <Resource/VSDA/VSDAManager.h>
#include <rpc/server.h>

static std::unique_ptr<::rpc::server> gRPC;
static std::unique_ptr<BG::API::VSDA::Manager> gMgr;

void StartClusterRPC(BG::Common::Logger::LoggingSystem* L,
                     Config::Config* C,
                     Server::Server* S)
{
    int port = C->GetInt("Network.RPCCallback.Port", 7999);
    gMgr = std::make_unique<BG::API::VSDA::Manager>(L, C, S);

    gRPC = std::make_unique<::rpc::server>(port);
    gRPC->bind("RegisterNode",
        [](const std::string& j){
            auto obj = nlohmann::json::parse(j);
            return gMgr->RegisterNode(obj["id"], obj["host"], obj["port"]).dump();
        });
    gRPC->bind("Heartbeat",
        [](const std::string& id){
            return gMgr->CheckIn(id, "", 0, false).dump();
        });
    gRPC->bind("GetClusterStatus",
        [](){ return gMgr->GetClusterStatus().dump(); });
    gRPC->bind("Ping", [](){ return "PONG"; });
    gRPC->async_run(4);
    L->Log("Cluster RPC ready on port " + std::to_string(port), 1);
}