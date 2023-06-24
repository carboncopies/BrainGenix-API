//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file provides a class for creating/registering/keeping track of restbed resources.
    Additional Notes: None
    Date Created: 2021-11-01
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <iostream>
#include <string>
#include <memory>
#include <cstdlib>
#include <functional>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <restbed>

// Internal Libraries (BG convention: use <> instead of "")
#include <Config/Config.h>
#include <Server/Server.h>


namespace BG {
namespace API {
namespace Server {


/**
 * @brief The resource manager class owns all restbed resources (handlers for endpoints).
 * It handles registering them, keeping track of them, and giving them a shared struct for keeping track of states.
 * 
 */
class ResourceManager {

private:

    Server Server_; /**Instance of the server struct, used to pass data to callbacks for restbed*/

public:

    /**
     * @brief Construct a new Resource Manager object
     * Creates all resources needed in the service.
     * 
     * @param _Service 
     * @param _Config 
     */
    ResourceManager(restbed::Service &_Service, Config::Config &_Config);

    /**
     * @brief Destroy the Resource Manager object
     * 
     */
    ~ResourceManager();


    void AddResource()

};



}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG