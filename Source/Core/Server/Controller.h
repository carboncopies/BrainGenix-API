//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file provides the class that manages the web service.
    Additional Notes: None
    Date Created: 2021-11-01
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <iostream>

// Third-Party Libraries (BG convention: use <> instead of "")

// Internal Libraries (BG convention: use <> instead of "")
#include <Config/Config.h>


namespace BG {
namespace API {
namespace Server {


/**
 * @brief This class manages the server code. It will initialize restbed with the given config and setup the routes.
 * 
 * @details Within this class, are subclasses each managing different aspects of the restbed service.
 * There will be a database connector class that will handle user authentication checks. Additionally, there will be a
 * route manager class that adds the routes as well as generating the json payload responses.
 */
class Controller {

public:

    /**
     * @brief Construct a new Controller object
     * The controller will setup the server based on the provided config pointer's data.
     * 
     * @param _Config 
     */
    Controller(Config::Config &_Config);


    /**
     * @brief Destroy the Controller object
     * 
     */
    ~Controller();

};



}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG