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



namespace BG {
namespace API {
namespace Config {

/**
 * @brief The configuration class stores the actual config data. Pretty self explanitory.
 * 
 */
struct Config {
    std::string ConfigFilePath; /**Path to program's configuration file*/

    int PortNumber;             /**Sets the port number that the service is running on.*/
    std::string Host;           /**Sets the host that the service binds to*/

    int NESPortNumber;          /**Port number to connect to the upstream NES service on*/
    std::string NESHost;        /**Host of the upstream NES service*/
    int NESTimeout_ms;          /**Set the timeout for the upstream NES service in milliseconds*/
};



}; // Close Namespace Config
}; // Close Namespace API
}; // Close Namespace BG