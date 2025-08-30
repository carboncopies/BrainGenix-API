//=============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//=============================================================//

#include <Resource/AppComponents.h>
#include <Resource/Controller.h>
#include <oatpp/network/Server.hpp>
#include <Main.h>
#include <Resource/VSDA/VSDAManager.h>   // VSDA manager already in tree
#include <rpc/server.h>                  // boost::asio based rpc

static std::unique_ptr<::rpc::server> gClusterRPC;
static std::unique_ptr<BG::API::VSDA::Manager> gClusterMgr;

void StartClusterRPC(BG::Common::Logger::LoggingSystem* L,
                     BG::API::Config::Config* C,
                     BG::API::Server::Server* S)
{
    int port = 7998;//C->GetInt("Network.RPCCallback.Port", 7999);
    gClusterMgr = std::make_unique<BG::API::VSDA::Manager>(L, C, S);

    gClusterRPC = std::make_unique<::rpc::server>(port);
    gClusterRPC->bind("RegisterNode",
        [](const std::string& jsonStr){
            auto obj = nlohmann::json::parse(jsonStr);
            return gClusterMgr->RegisterNode(obj["id"], obj["host"], obj["port"]).dump();
        });
    gClusterRPC->bind("Heartbeat",
        [](const std::string& id){
            return gClusterMgr->CheckIn(id, "", 0, false).dump();
        });
    gClusterRPC->bind("GetClusterStatus",
        [](){ return gClusterMgr->GetClusterStatus().dump(); });
    gClusterRPC->bind("Ping", [](){ return "PONG"; });
    gClusterRPC->async_run(4);
    L->Log("Cluster RPC ready on port " + std::to_string(port), 1);
}

int main(int NumArguments, char** ArgumentValues) {
    BG::Common::Logger::LoggingSystem Logger;

    // Startup With Config Manager, Will Read Args And Config File, Then Parse Into Config Struct
    BG::API::Config::Manager ConfigManager(&Logger, NumArguments, ArgumentValues);
    BG::API::Config::Config& SystemConfiguration = ConfigManager.GetConfig();

    BG::API::Server::Server ServerData{};

    // 1) Start VSDA cluster RPC (port 7999)
    StartClusterRPC(&Logger, &SystemConfiguration, &ServerData);

    // 2) Keep existing managers
    BG::API::RPC::Manager RPCManager(&Logger, &SystemConfiguration, &ServerData);
    BG::API::API::RPCManager RPCServer(&SystemConfiguration, &Logger, &ServerData);

    // Bind VSDA RPC endpoints (HTTP side)
    RPCServer.BindVSDAEndpoints(nullptr); // we expose cluster via RPC, not HTTP

    oatpp::base::Environment::init();
    AppComponent components(&SystemConfiguration);

    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
    router->addController(std::make_shared<BrainGenixAPIController>(&ServerData, &RPCManager, gClusterMgr.get()));

    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

    oatpp::network::Server server(connectionProvider, connectionHandler);

    Logger.Log("BrainGenix-API Server Starting", 1);
    server.run();
    Logger.Log("BrainGenix-API Server Stopping", 1);

    oatpp::base::Environment::destroy();
    return 0;
}