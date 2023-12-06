//=====================================================//
// This file is part of the BrainGenix Shared Codebase //
//=====================================================//

/*
    Description: This file provides the configuration struct for the logging subsystem.
    Additional Notes: None
    Date Created: 2023-06-25
*/


#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <string>
#include <stdint.h>

// Third-Party Libraries (BG convention: use <> instead of "")

// Internal Libraries (BG convention: use <> instead of "")



namespace BG {
namespace Shared {
namespace Logger {

/**
 * @brief The configuration class stores parameters used to configure the logging system.
 * 
 */
struct Config {
    std::string LogfilePath;    /**Path to location of primary logfile*/
    uint64_t    LogfileSize;    /**Max logfile size before the file is rotated out*/
    uint16_t    MaxLogfiles;    /**Max number of logfiles before old ones are deleted*/
};



}; // Close Namespace Logger
}; // Close Namespace Shared
}; // Close Namespace BG