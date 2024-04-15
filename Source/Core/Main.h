//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This is the main file of the system.
    Additional Notes: None
    Date Created: 2021-11-01
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <iostream>

// Third-Party Libraries (BG convention: use <> instead of "")
// #include <rpc/client.h>

// Internal Libraries (BG convention: use <> instead of "")
#include <Config/ConfigurationManager.h>
#include <Server/Controller.h>
#include <NeuroglancerWrapper/NeuroglancerWrapper.h>



#include <RPC/ClientManager.h>
#include <RPC/RPCManager.h>



/**
 * Main function.
*/
int main(int NumArguments, char** ArgumentValues);