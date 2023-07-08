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

    try
    {
        auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");
    }
    catch (const spdlog::spdlog_ex &ex)
    {
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }

    auto console = spdlog::stdout_color_mt("console");
    auto err_logger = spdlog::stderr_color_mt("stderr");
    spdlog::get("console")->info("loggers can be retrieved from a global registry using the spdlog::get(logger_name)");

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


    // Now add the logging methods here where users can pass in std::string, const char*, etc. and set the log severity


};


}; // Close Namespace Logger
}; // Close Namespace Shared
}; // Close Namespace BG