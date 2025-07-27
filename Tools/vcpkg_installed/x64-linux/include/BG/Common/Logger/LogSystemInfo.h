//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <memory.h>

// Third-Party Libraries (BG convention: use <> instead of "")

// Internal Libraries (BG convention: use <> instead of "")
#include <BG/Common/Logger/Logger.h>

// #include <Version.h>
// #include <CompilePlatformInfo.h>
// #include <CompileTimeStamp.h>




namespace BG {
namespace Common {
namespace Logger {


/**
 * @brief Logs the system information as defined during compilation (version, target os, etc.)
 * 
 */
void LogSystemInfo(LoggingSystem* Logger);


}; // Close Namespace Logger
}; // Close Namespace Common
}; // Close Namespace BG