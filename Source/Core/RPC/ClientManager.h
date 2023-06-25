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

    std::unique_ptr<::rpc::client> NESClient_; /**Client to upstream NES Service*/

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