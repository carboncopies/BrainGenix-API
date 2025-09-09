//=============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//=============================================================//

/*
    Description: This is the main file of the system.
    Additional Notes: None
    Date Created: 2021-11-01
*/

#include <Resource/AppComponents.h>

#include <Resource/Controller.h>

#include <oatpp/network/Server.hpp>
#include <Main.h>

#include <Cluster/VSDA/VSDAConnectionManager.h>

int main(int NumArguments, char** ArgumentValues) {
    BG::Common::Logger::LoggingSystem Logger;

    // Initialize Config Manager
    Logger.Log("[MAIN] Loading System Configuration", 5);
    ConfigParser ConfigManager("Config.yaml", NumArguments, ArgumentValues, &Logger);
    Logger.Log("[MAIN] System Configuration Loaded", 5);

    Server ServerData{};

    RPCClientManager RPCClientManager(&Logger, &ConfigManager, &ServerData); 

    // The manager is for internal RPC calls (i.e. NES->EVM, or EVM->NES, NOT to the user)
    RPCManager RPCServer(&ConfigManager, &Logger, &ServerData);


    // Create and initialize the VSDA connection manager
    VSDAConnectionManager VSDAManager(&Logger, &RPCServer);
    VSDAManager.Initialize();
    // ServerData.Manager_ = &VSDAManager;
    

    oatpp::base::Environment::init();
    
    // init oatpp's components
    AppComponent components(&ConfigManager);

    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
    
    router->addController(std::make_shared<BrainGenixAPIController>(&ServerData, &RPCClientManager, &VSDAManager));
    
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

    oatpp::network::Server server(connectionProvider, connectionHandler);

    server.run();

    oatpp::base::Environment::destroy();
}