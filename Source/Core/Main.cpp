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

    BG::API::NeuroglancerWrapper NeuroWrapper(SystemConfiguration, &Logger);

    // Setup Upstream API Connection Handler
    BG::API::RPC::Manager RPCManager(&Logger, &SystemConfiguration, Server);
    BG::API::API::RPCManager RPCServer(&SystemConfiguration, &Logger, Server);

    std::cout<<NeuroWrapper.GetNeuroglancerURL("http://localhost:9000/NeuroglancerDataset")<<std::endl;
    std::cout<<NeuroWrapper.GetNeuroglancerURL("http://localhost:9000/NeuroglancerDataset1")<<std::endl;
    std::cout<<NeuroWrapper.GetNeuroglancerURL("http://localhost:9000/NeuroglancerDataset2")<<std::endl;
    std::cout<<NeuroWrapper.GetNeuroglancerURL("http://localhost:9000/NeuroglancerDataset3")<<std::endl;
    std::cout<<NeuroWrapper.GetNeuroglancerURL("http://localhost:9000/NeuroglancerDataset4")<<std::endl;
    std::cout<<NeuroWrapper.GetNeuroglancerURL("http://localhost:9000/NeuroglancerDataset5")<<std::endl;
    std::cout<<NeuroWrapper.GetNeuroglancerURL("http://localhost:9000/NeuroglancerDataset6")<<std::endl;
    std::cout<<NeuroWrapper.GetNeuroglancerURL("http://localhost:9000/NeuroglancerDataset7")<<std::endl;
    std::cout<<NeuroWrapper.GetNeuroglancerURL("http://localhost:9000/NeuroglancerDataset8")<<std::endl;
    std::cout<<NeuroWrapper.GetNeuroglancerURL("http://localhost:9000/NeuroglancerDataset9")<<std::endl;
    std::cout<<NeuroWrapper.GetNeuroglancerURL("http://localhost:9000/NeuroglancerDataset0")<<std::endl;
    std::cout<<NeuroWrapper.GetNeuroglancerURL("http://localhost:9000/NeuroglancerDataset-")<<std::endl;
    std::cout<<NeuroWrapper.GetNeuroglancerURL("http://localhost:9000/NeuroglancerDataseta")<<std::endl;
    std::cout<<NeuroWrapper.GetNeuroglancerURL("http://localhost:9000/NeuroglancerDatasetas")<<std::endl;
    std::cout<<NeuroWrapper.GetNeuroglancerURL("http://localhost:9000/NeuroglancerDatasets")<<std::endl;
    std::cout<<NeuroWrapper.GetNeuroglancerURL("http://localhost:9000/NeuroglancerDatasetd")<<std::endl;
    std::cout<<NeuroWrapper.GetNeuroglancerURL("http://localhost:9000/NeuroglancerDatasetf")<<std::endl;

    // Start Server
    ServerController.StartService();
    ServerController.HangUntilExit();

}


