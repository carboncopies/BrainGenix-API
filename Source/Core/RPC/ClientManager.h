//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file manages the RPC client.
    Additional Notes: None
    Date Created: 2023-06-24
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <rpc/client.h>
#include <rpc/rpc_error.h>

// Internal Libraries (BG convention: use <> instead of "")
#include <Version.h>

#include <Config/Config.h>
#include <Server/Server.h>
#include <Util/Types.h>


namespace BG {
namespace API {
namespace RPC {


/**
 * @brief Manager class for the database - will take a config struct (need to make this), and then connect to the db from that struct.
 * Then, methods here can be added to call sql commands as needed.
 * 
 */
class Manager {

private:
    Config::Config* Config_; /**Pointer to the configuration instance*/
    Server::Server* Server_; /**Pointer to server struct, this class updates upstream status info*/

    bool RequestThreadsExit_; /**Used to signal to threads that they should exit*/
    std::thread ConnectionManagerNES_; /**Thread running the NES connection manager*/

    std::unique_ptr<::rpc::client> NESClient_; /**Client to upstream NES Service*/

    /**
     * @brief Attempts to connect to the NES client. On failure, returns false.
     * NOTE: THE NES CLIENT MAY NOT BE USED IF THIS FAILS. IF THIS FAILS, THE CLIENT WILL BE SET TO NULL!
     * DO NOT USE IT IF IT IS NULL - for obvious reasons.
     * 
     * @return true 
     * @return false 
     */
    bool ConnectNES();

    /**
     * @brief This function is run in another thread and checks/reconnects/updates info/connection data about the NES client.
     * 
     */
    void ConnectionManagerNES();

    /**
     * @brief Uses the client to run a version check, updates the server status afterwards.
     * Returns true whenever everything is good, false if degraded/failed/otherwise.
     * 
     * @return true 
     * @return false 
     */
    bool RunVersionCheckNES();

public:


    /**
     * @brief Create an RPC client manager, used to talk to upstream services
     * Will read the configuration passed in to figure out who to connect with
     * 
     * @param _Config 
     * @param _Server 
     */
    Manager(Config::Config* _Config, Server::Server* _Server);

    /**
     * @brief Destroy the Manager object
     * 
     */
    ~Manager();

};


}; // Close Namespace DB
}; // Close Namespace API
}; // Close Namespace BG