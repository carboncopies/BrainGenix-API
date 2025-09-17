#pragma once

#include <BG/Common/Logger/Logger.h>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

#include <Cluster/VSDA/VSDAConnectionManager.h>

#include "ConfigParser.h"



/**
 * @class ConfigManager
 * @brief Manages logger, config parser, and API hook instances with VSDA leader callback support.
 * 
 * This class provides a centralized way to manage core system components and
 * registers a callback method that the VSDA leader can call to retrieve configuration.
 */
class ConfigManager {
public:
    /**
     * @brief Constructor for ConfigManager
     * 
     * @param _Logger Pointer to the logging system instance
     * @param _Config Pointer to the configuration parser instance
     * @param _RPCManger Pointer to the RPC manager instance
     */
    ConfigManager(BG::Common::Logger::LoggingSystem* _Logger,
                  ConfigParser* _Config,
                  VSDAConnectionManager* _VSDAConnectionManager);

    /**
     * @brief Destructor
     */
    ~ConfigManager();

    /**
     * @brief Initialize the ConfigManager and register VSDA callback
     */
    void Initialize();

    /**
     * @brief Get the logger instance
     * @return Pointer to the logging system
     */
    BG::Common::Logger::LoggingSystem* GetLogger() const { return Logger_; }

    /**
     * @brief Get the config parser instance
     * @return Pointer to the configuration parser
     */
    ConfigParser* GetConfigParser() const { return ConfigParser_; }

    /**
     * @brief Get the RPC manager instance
     * @return Pointer to the RPC manager
     */
     VSDAConnectionManager* GetVSDAConnectionManager() const { return VSDAConnectionManager_; }

    /**
     * @brief Callback method that VSDA leader can call to get configuration
     * 
     * This method returns a JSON object containing relevant configuration data
     * that the VSDA leader might need.
     * 
     * @param _Request JSON request string (can be empty for full config)
     * @return JSON string containing configuration data
     */
    std::string GetConfigurationCallback(const std::string& _Request = "");

private:
    BG::Common::Logger::LoggingSystem* Logger_;
    ConfigParser* ConfigParser_;
    VSDAConnectionManager* VSDAConnectionManager_;
    
    bool Initialized_;
    
};
