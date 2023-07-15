//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file provides the configuration struct passed around to other subsystems.
    Additional Notes: None
    Date Created: 2023-06-23
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <string>
#include <vector>

// Third-Party Libraries (BG convention: use <> instead of "")

// Internal Libraries (BG convention: use <> instead of "")
#include <Config/ConfigDefaults.h>


namespace BG {
namespace API {
namespace Config {

/**
 * @brief The configuration class stores the actual config data. Pretty self explanitory.
 * 
 */
struct Config {
    std::vector<std::string> ConfigFilePaths = {
        CONFIG_DEFAULT_CFG_FILE_PATH1,
        CONFIG_DEFAULT_CFG_FILE_PATH2
    };  /**List of potential path to program's configuration file*/

    int PortNumber = CONFIG_DEFAULT_PORT_NUMBER;                /**Sets the port number that the service is running on.*/
    std::string Host = CONFIG_DEFAULT_HOST;                     /**Sets the host that the service binds to*/

    int NESPortNumber = CONFIG_DEFAULT_NES_PORT_NUMBER;         /**Port number to connect to the upstream NES service on*/
    std::string NESHost = CONFIG_DEFAULT_NES_HOST;              /**Host of the upstream NES service*/
    int NESTimeout_ms = CONFIG_DEFAULT_NES_TIMEOUT_MS;          /**Set the timeout for the upstream NES service in milliseconds*/
};



}; // Close Namespace Config
}; // Close Namespace API
}; // Close Namespace BG