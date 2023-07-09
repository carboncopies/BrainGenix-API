//=====================================================//
// This file is part of the BrainGenix Shared Codebase //
//=====================================================//

/*
    Description: This file provides the main interface to the logging system's wrapper.
    Additional Notes: None
    Date Created: 2023-06-25
*/

#pragma once


// Standard Libraries (BG convention: use <> instead of "")

// Third-Party Libraries (BG convention: use <> instead of "")
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// Internal Libraries (BG convention: use <> instead of "")
#include <Logger/Config.h>


namespace BG {
namespace Shared {
namespace Logger {


/**
 * @brief This class provides the shared logging service used in all BG subsystems.
 * The class is just a wrapper for spdlog, but we write a wrapper so functionality may be easily extended in the future.
 * 
 */
class Logger {

private:
    Config Config_; /**Local copy of the logging configuration struct*/

public:

    /**
     * @brief Construct a new Logger object
     * The logger will require a valid logging configuration to work.
     * 
     * @param _Config 
     */

    Logger(Config _Config);
    /**
     * @brief Destroy the Logger object
     * 
     */
    ~Logger();


     /**
     * @brief Method to pass in std::string, const char*, etc. and set the log severity
     *
     */
     bool Log(std::string _Message, int _LogLevel);

};


}; // Close Namespace Logger
}; // Close Namespace Shared
}; // Close Namespace BG