//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file manages parsing cli arguments.
    Additional Notes: None
    Date Created: 2023-06-23
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <iostream>
#include <fstream>
#include <iterator>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <boost/program_options.hpp>

// Internal Libraries (BG convention: use <> instead of "")
#include <Config/Config.h>

namespace BG {
namespace API {
namespace Config {

/**
 * @brief Argument parsing class, does what the name says. Is responsible for editing the config class as needed.
 * 
 * @details This class is given a configuration object from the configuration manager where it populates any supported arguments.
 * From there, the data is then passed back to the config manager for populating with config file data through another class.
 */
class ArgumentParser {

public:

   ArgumentParser();

};

}; // Close Namespace Config
}; // Close Namespace API
}; // Close Namespace BG