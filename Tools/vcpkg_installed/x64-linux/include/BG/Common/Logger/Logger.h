//=================================//
// This file is part of BrainGenix //
//=================================//

/*
    Description: This file contains the Logger interface.
    Additional Notes: None
    Date Created: 2023-07-05
    Author(s): Thomas Liao


    Copyright (C) 2023  Thomas Liao

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/


#pragma once


// Standard Libraries (BG convention: use <> instead of "")
#include <iostream>
#include <string>
#include <ctime>
#include <map>
#include <list>
#include <array>
#include <mutex>
#include <fstream>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <yaml-cpp/yaml.h>

// Internal Libraries (BG convention: use <> instead of "")
#include <BG/Common/Logger/RGBColor.h>




namespace BG {
namespace Common {
namespace Logger {



/**
 * @brief Main Logging System, Saves Logs To Database/Text File If Configured To.
 */
class LoggingSystem {

private: 

    YAML::Node LocalSystemConfiguration;


    bool PrintLogOutput = true;
    bool ColorizeLog = true;
    bool ReplaceLevelWithText = false;
    bool UseTextLogLevel_ = false;
    bool KeepVectorOfLogMessages_ = false;
    int MinimumLogLevel = 5;

    bool EnableLogFile_            = true; /**Enables or disables saving logs to a file locally*/
    std::string LogFilePathPrefix_ = "";   /**Path prefix for log file, is relative to the working directory ers is run with*/
    std::ofstream FileStream_;             /**Filestream used to write the log to disk, if not enabled this will never be touched at runtime*/


    int LogLevelTargetWidth = 6;
    int LogTimeTargetWidth = 19;
    const char* InsertString = "                                                         ";
    
    std::mutex LogMutex_; /**<Used to ensure thread safety during vector operations*/


    std::map<int, RGBColor> ColorLookup_; /*!< Lookup for converting log level to RGB values (stored in RGBColor struct). Populated based on values stored in Config.yaml */
    std::map<int, std::string> LogNameLookup_; /*!< Lookup for converting log level to name of log level (See Config.yaml for values). */


    /**
     * @brief Print out text to std::cout.
     * 
     * @param Message Message to be printed.
     * @param LogLevel Level of log (looked up in map to convert to RGB values).
     */
    void ColorizeText(std::string Message, int LogLevel);


    /**
     * @brief Add item to system log. 
     * 
     * @param LogItem Text to log.
     * @param LogLevel Importance of log entry (consult Config.yaml for more info).
     */
    void LogItem(const char* LogItem, int LogLevel=5);

    /**
     * @brief Writes a given line (including the '\n') to disk with the given file.
     * 
     * @param Line 
     */
    void WriteLineToFile(std::string Line);

public:

    std::vector<std::string> LogMessages_; /**<List Of Messages*/
    std::vector<std::string> FullLogMessages_; /**<Full Messages*/
    std::vector<int> LogLevels_; /**<List of Log Levels*/
    std::vector<std::string> LogTimes_; /**<List Of Times*/
    std::vector<RGBColor> LogColors_; /**<List of Colors For Messages*/




    /**
     * @brief Setup the logger class (connect to db if needed, open file for writing if enabled, etc.)
     * 
     * @param SystemConfiguration System configuration file in YAML::Node format from Config.yaml.
     */
    LoggingSystem(YAML::Node SystemConfiguration);

    /**
     * @brief Super simplified constructor for use in init tests - (where systems need to create an instance of the logger for testing their stuff, not tests for the logging system itself.)
    */
    LoggingSystem(bool _EnableOutput);

    /**
     * @brief Default colorized logging system instance, uses default config found in ers yaml file.
     * 
     */
    LoggingSystem();


    /**
     * @brief Cleanup the logger class, close db connection, close file io, etc.
     * 
     */
    ~LoggingSystem();


    /**
     * @brief Enable or disable us keeping logs in vectors in memory.
     * 
     * @param _NewState 
     */
    void SetKeepVectorLogs(bool _NewState);

    /**
     * @brief Overload allowing the user to enable/disable the log by passing in a bool to the enable param
     * 
     * @param LogMessage 
     * @param LogLevel 
     * @param Enable 
     */
    void Log(std::string LogMessage, int LogLevel=5, bool Enable=true);

    /**
     * @brief Overload allowing the user to enable/disable the log by passing in a bool to the enable param
     * 
     * @param LogMessage 
     * @param LogLevel 
     * @param Enable 
     */
    void Log(const char* LogMessage, int LogLevel=5, bool Enable=true);

};





}; // Close Namespace Logger
}; // Close Namespace Common
}; // Close Namespace BG