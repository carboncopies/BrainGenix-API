//=============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//=============================================================//

/*
    Description: This is the main file of the system.
    Additional Notes: None
    Date Created: 2021-11-01
*/


#include <Main.h>


// todo:
/**

delegate - add system to automatically detect dropped connections and try and re-establish a link
delegate - replace std::cout statements with logger calls once prishita writes the logging system

 * 
 */


int main(int NumArguments, char** ArgumentValues) {

    // Startup With Config Manager, Will Read Args And Config File, Then Parse Into Config Struct
    BG::API::Config::Manager ConfigManager(NumArguments, ArgumentValues);
    BG::API::Config::Config& SystemConfiguration = ConfigManager.GetConfig();

    // Setup Logger Here
    std::unique_ptr<BG::Common::Logger::LoggingSystem> Logger = std::make_unique<BG::Common::Logger::LoggingSystem>();

    // Setup Server
    BG::API::Server::Controller ServerController(SystemConfiguration);
    BG::API::Server::Server* Server = ServerController.GetServerStruct();

    // Setup Upstream API Connection Handler
    BG::API::RPC::Manager RPCManager(std::move(Logger),&SystemConfiguration, Server);

    // Start Server
    ServerController.StartService();

}


