//=============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//=============================================================//

/*
    Description: This is the main file of the system.
    Additional Notes: None
    Date Created: 2021-11-01
*/

#include <Resource/Dataset/Dataset.h>
#include <Resource/AppComponents.h>
#include <Resource/Controller.h>
#include <oatpp/network/Server.hpp>
#include <Main.h>

int main(int NumArguments, char** ArgumentValues) {
    BG::Common::Logger::LoggingSystem Logger;

    // Startup With Config Manager, Will Read Args And Config File, Then Parse Into Config Struct
    BG::API::Config::Manager ConfigManager(&Logger, NumArguments, ArgumentValues);
    BG::API::Config::Config& SystemConfiguration = ConfigManager.GetConfig();

    BG::API::Server::Server ServerData{};

    BG::API::RPC::Manager RPCManager(&Logger, &SystemConfiguration, &ServerData); 

    // The manager is for internal RPC calls (i.e. NES->EVM, or EVM->NES, NOT to the user)
    BG::API::API::RPCManager RPCServer(&SystemConfiguration, &Logger, &ServerData);

    oatpp::base::Environment::init();
    
    // init oatpp's components
    AppComponent components(&SystemConfiguration);

    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
    router->addController(std::make_shared<BrainGenixAPIController>(&ServerData, &RPCManager));

    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

    oatpp::network::Server server(connectionProvider, connectionHandler);

    server.run();

    oatpp::base::Environment::destroy();
}


