//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file provides the class that manages the web service.
    Additional Notes: None
    Date Created: 2024-04-12
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <regex>

// Third-Party Libraries (BG convention: use <> instead of "")

// Internal Libraries (BG convention: use <> instead of "")
#include <Config/Config.h>

#include <BG/Common/Logger/Logger.h>



namespace BG {
namespace API {


/**
 * @brief This class manages the server code. It will initialize restbed with the given config and setup the routes.
 * 
 * @details Within this class, are subclasses each managing different aspects of the restbed service.
 * There will be a database connector class that will handle user authentication checks. Additionally, there will be a
 * route manager class that adds the routes as well as generating the json payload responses.
 */
class NeuroglancerWrapper {

private:



public:

    /**
     * @brief Construct a new Controller object
     * The controller will setup the server based on the provided config pointer's data.
     * 
     * @param _Config 
     */
    NeuroglancerWrapper(Config::Config& _Config, BG::Common::Logger::LoggingSystem* _Logger);


    /**
     * @brief Destroy the Controller object
     * 
     */
    ~NeuroglancerWrapper();


    

};



}; // Close Namespace API
}; // Close Namespace BG