//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file is where all routes are linked into the system
    Additional Notes: None
    Date Created: 2023-06-23
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <string>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <restbed>

// Internal Libraries (BG convention: use <> instead of "")
#include <Server/Server.h>

#include <Resource/Hello.h>


namespace BG {
namespace API {
namespace Server {

class EndpointManager {

private:

    std::vector<std::unique_ptr<void>> Routes_; /**List containing void pointers to all of the route classes*/

public:

    /**
     * @brief Construct a new Endpoint Manager object
     * 
     */
    EndpointManager();

    /**
     * @brief Destroy the Endpoint Manager object
     * 
     */
    ~EndpointManager();


    /**
     * @brief This function is where all routes are added to the service.
     * They're included in this file from the Route directory and then added in the AddRoutes function body.
     * 
     * @param _Server 
     */
    void AddRoutes(restbed::Service &_Service, Server::Server &_Server);


};

}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG