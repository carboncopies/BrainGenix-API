/*
 * Example usage of ConfigManager class
 * 
 * This file demonstrates how to use the ConfigManager class to manage
 * logger, config parser, and API hook instances, and how the VSDA leader
 * can call the configuration callback.
 */

#include <Config/ConfigManager.h>
#include <BG/Common/Logger/Logger.h>
#include <Config/ConfigParser.h>
#include <RPC/RPCManager.h>
#include <iostream>

void ExampleUsage() {
    // Create core components
    BG::Common::Logger::LoggingSystem Logger;
    ConfigParser Config("config.yaml", 0, nullptr, &Logger);
    Server ServerData{};
    RPCManager RPCServer(&Config, &Logger, &ServerData);
    
    // Create and initialize ConfigManager
    ConfigManager ConfigManagerInstance(&Logger, &Config, &RPCServer);
    ConfigManagerInstance.Initialize();
    
    // Now the VSDA leader can call "GetConfiguration" RPC method to get config
    // Example of what the VSDA leader would receive:
    
    // Full configuration request:
    std::string fullConfig = ConfigManagerInstance.GetConfigurationCallback("");
    
    // Specific configuration request:
    std::string specificRequest = R"({"include": ["advanced"]})";
    std::string advancedConfig = ConfigManagerInstance.GetConfigurationCallback(specificRequest);
    
    // Access individual components through ConfigManager
    BG::Common::Logger::LoggingSystem* logger = ConfigManagerInstance.GetLogger();
    ConfigParser* config = ConfigManagerInstance.GetConfigParser();
    RPCManager* rpcManager = ConfigManagerInstance.GetRPCManager();
    
    // Use the components as needed...
    logger->Log("[Example] ConfigManager usage demonstrated", 5);
}

/*
 * Expected JSON output from GetConfigurationCallback:
 * 
 * {
 *   "system": {
 *     "name": "BrainGenix-API",
 *     "version": "1.0.0",
 *     "environment": "development"
 *   },
 *   "rpc": {
 *     "host": "localhost",
 *     "port": 7999,
 *     "timeout": 5000
 *   },
 *   "vsda": {
 *     "rpcPort": 8003,
 *     "timeout": 5000,
 *     "advertisedHost": "localhost",
 *     "enableHealthCheck": true
 *   },
 *   "database": {
 *     "host": "localhost",
 *     "port": 5432,
 *     "name": "braingenix",
 *     "username": ""
 *   },
 *   "api": {
 *     "host": "localhost",
 *     "port": 8080,
 *     "maxConnections": 1000
 *   },
 *   "timestamp": 1234567890
 * }
 */
