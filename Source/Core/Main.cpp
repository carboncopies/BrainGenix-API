//=============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//=============================================================//

/*
    Description: This is the main file of the system.
    Additional Notes: None
    Date Created: 2021-11-01
*/


#include <Main.h>



int main(int NumArguments, char** ArgumentValues) {


    // Setup Logger Here
    BG::Common::Logger::LoggingSystem Logger;

    // Startup With Config Manager, Will Read Args And Config File, Then Parse Into Config Struct
    BG::API::Config::Manager ConfigManager(&Logger, NumArguments, ArgumentValues);
    BG::API::Config::Config& SystemConfiguration = ConfigManager.GetConfig();

    // Setup Server
    BG::API::Server::Controller ServerController(SystemConfiguration, &Logger);
    BG::API::Server::Server* Server = ServerController.GetServerStruct();

    // Setup Upstream API Connection Handler
    BG::API::RPC::Manager RPCManager(&Logger, &SystemConfiguration, Server);
    BG::API::API::RPCManager RPCServer(&SystemConfiguration, &Logger);

    // Start Server
    ServerController.StartService();
    ServerController.HangUntilExit();

}


