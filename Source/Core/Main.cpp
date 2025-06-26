//=============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//=============================================================//

/*
    Description: This is the main file of the system.
    Additional Notes: None
    Date Created: 2021-11-01
*/


#include <Resource/Dataset/Dataset.h>


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

    BG::API::NeuroglancerWrapper NeuroWrapper(SystemConfiguration, &Logger);

    // Setup Upstream API Connection Handler
    BG::API::RPC::Manager RPCManager(&Logger, &SystemConfiguration, Server); // This is what actually serves to the user, and does not handle internal routing
    BG::API::API::RPCManager RPCServer(&SystemConfiguration, &Logger, Server); // The manager is for internal RPC calls (i.e. NES->EVM, or EVM->NES, NOT to the user)


    BG::API::Resource::Dataset::Route Dataset(Server, &RPCManager, ServerController.Service_);


    // Start Server
    ServerController.StartService();
    ServerController.HangUntilExit();

}


