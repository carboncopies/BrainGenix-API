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
#include <oatpp-openssl/server/ConnectionProvider.hpp>
#include <oatpp-openssl/Config.hpp>
#include <Main.h>

int main(int NumArguments, char** ArgumentValues) {
    BG::Common::Logger::LoggingSystem Logger;

    // Startup With Config Manager, Will Read Args And Config File, Then Parse Into Config Struct
    BG::API::Config::Manager ConfigManager(&Logger, NumArguments, ArgumentValues);
    BG::API::Config::Config& SystemConfiguration = ConfigManager.GetConfig();

    // init global server struct
    BG::API::Server::Server Server{};
    g_Server = &Server; // NOTE: we can't pass the server pointer into oatpp's endpoints, so we need a global var

    oatpp::base::Environment::init();
    
    // init oatpp's components
    AppComponent components;

    // get router component
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

    router->addController(std::make_shared<BrainGenixAPIController>());

    /* Get connection handler component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

    /* Get connection provider component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

    /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
    oatpp::network::Server server(connectionProvider, connectionHandler);

    /* Priny info about server port */
    OATPP_LOGI("BrainGenix-API", "Server running on port %s", connectionProvider->getProperty("port").getData());

    BG::API::RPC::Manager RPCManager(&Logger, &SystemConfiguration, &Server); 
    g_Manager = &RPCManager;

    // The manager is for internal RPC calls (i.e. NES->EVM, or EVM->NES, NOT to the user)
    BG::API::API::RPCManager RPCServer(&SystemConfiguration, &Logger, &Server);

    /* Run server */
    server.run();

    oatpp::base::Environment::destroy();
}


