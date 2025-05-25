//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file provides a class for managing configuration data from all sources.
    Additional Notes: None
    Date Created: 2023-06-23
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <iostream>
#include <fstream>
#include <iterator>
#include <utility>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <boost/program_options.hpp>

// Internal Libraries (BG convention: use <> instead of "")
#include <Config/ArgumentParser.h>
#include <Config/ConfigFileParser.h>
#include <Config/Config.h>



namespace BG {
namespace API {
namespace Config {

/**
 * @brief Configuration Manager Class - Provides program configuration capabillities to the API server.
 * 
 * @details This class manages the argument and config file parsers, which each generate a config struct 
 * which is given to the rest of the system. This class manages creating and owning that config struct.
 */
class Manager {

private:

    Config Config_; /**Configuration struct instance, owned by this class and given to others as needed*/

public:

    /**
     * @brief Construct a new Manager object.
     * 
     * Initializes the Manager with a logger instance and command-line arguments for parsing.
     * 
     * @param _Logger Pointer to the logging system instance.
     * @param _NumArgs The number of command-line arguments.
     * @param _Args The array of command-line arguments.
     */
    Manager(BG::Common::Logger::LoggingSystem* _Logger, int _NumArgs, char** _Args);

    /**
     * @brief Destroy the Manager object.
     */
    ~Manager();

    /**
     * @brief Returns a reference to the configuration struct.
     * 
     * @return Config& A reference to the configuration struct.
     */
    Config& GetConfig();

};

}; // Close Namespace Config
}; // Close Namespace API
}; // Close Namespace BG