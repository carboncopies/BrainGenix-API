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

    // Startup With Config Manager, Will Read Args And Config File, Then Parse Into Config Struct
    BG::API::Config::Manager ConfigManager(NumArguments, ArgumentValues);
    BG::API::Config::Config& SystemConfiguration = ConfigManager.GetConfig();

    rpc::client c("127.0.0.1", 8001);

    for (unsigned int i = 0; i < 10000000; i++) {
        int a = c.call("add", 2, 3).as<int>();
        if (i%10000==0) {
            std::cout<<i<<std::endl;
        }
    }

    // Setup Server
    BG::API::Server::Controller ServerController(SystemConfiguration);

    // Start Server
    ServerController.StartService();

}

