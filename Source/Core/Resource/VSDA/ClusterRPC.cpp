#include "ClusterRPC.h"
#include <Resource/VSDA/VSDAManager.h>
#include <rpc/server.h>

// static std::unique_ptr<::rpc::server> gRPC;
// static std::unique_ptr<BG::API::VSDA::Manager> gMgr;

// void StartClusterRPC(BG::Common::Logger::LoggingSystem* L,
//                      Config::Config* C,
//                      Server::Server* S)
// {
//     int port = 7998;//C->GetInt("Network.RPCCallback.Port", 7999);
//     gMgr = std::make_unique<BG::API::VSDA::Manager>(L, C, S);

//     gRPC = std::make_unique<::rpc::server>(port);
//     gRPC->bind("RegisterNode",
//         [](const std::string& j, const std::string& client_addr){
//             auto obj = nlohmann::json::parse(j);
//             std::string host = obj["host"];
            
//             // Use client address if node reports localhost
//             if (host == "127.0.0.1" || host == "localhost") {
//                 size_t colon_pos = client_addr.find_last_of(':');
//                 host = client_addr.substr(0, colon_pos);
//             }
            
//             return gMgr->RegisterNode(obj["id"], host, obj["port"]).dump();
//         });
//     gRPC->bind("Heartbeat",
//         [](const std::string& id){
//             return gMgr->CheckIn(id, "", 0, false).dump();
//         });
//     gRPC->bind("GetClusterStatus",
//         [](){ return gMgr->GetClusterStatus().dump(); });
//     gRPC->bind("Ping", [](){ return "PONG"; });
//     gRPC->async_run(4);
//     L->Log("Cluster RPC ready on port " + std::to_string(port), 1);
// }