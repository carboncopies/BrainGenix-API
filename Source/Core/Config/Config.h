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
#include <Config/ConfigDefaults.h>


namespace BG {
namespace API {
namespace Config {

/**
 * @struct Config
 * @brief The configuration struct stores the actual configuration data used by the system.
 */
struct Config {
    /**
     * @brief List of potential paths to the program's configuration file.
     */
    std::vector<std::string> ConfigFilePaths = {
        CONFIG_DEFAULT_CFG_FILE_PATH1,
        CONFIG_DEFAULT_CFG_FILE_PATH2
    };  /**List of potential path to program's configuration file*/

    /**
     * @brief Sets the port number that the service is running on.
     */
    int PortNumber = CONFIG_DEFAULT_PORT_NUMBER;                /**Sets the port number that the service is running on.*/

    /**
     * @brief Sets the host that the service binds to.
     */
    std::string Host = CONFIG_DEFAULT_HOST;                     /**Sets the host that the service binds to*/

    /**
     * @brief The host for RPC callbacks.
     */
    std::string RPCCallbackHost = "localhost";

    /**
     * @brief The port for RPC callbacks.
     */
    int RPCCallbackPort = 7999;
    /**
     * @brief Configures whether SSL/HTTPS is used.
     * 
     * Using HTTPS requires specifying a server key and certificate file.
     */
    bool UseHTTPS = CONFIG_DEFAULT_USE_HTTPS;                   /**Configure if we do or do not use SSL/HTTPS. Using it requires having a server key and crt file specified*/
    std::string KeyFilePath;                                    /**Path to the key file*/
    std::string CrtFilePath;                                    /**Path the the crt file*/
    // std::string PemFilePath;                                    /**Path to the pem file*/

    /**
     * @brief Port number to connect to the upstream NES service on.
     */
    int NESPortNumber = CONFIG_DEFAULT_NES_PORT_NUMBER;         /**Port number to connect to the upstream NES service on*/
    /**
     * @brief Host of the upstream NES service.
     */
    std::string NESHost = CONFIG_DEFAULT_NES_HOST;              /**Host of the upstream NES service*/
    /**
     * @brief Set the timeout for the upstream NES service in milliseconds.
     */
    int NESTimeout_ms = CONFIG_DEFAULT_NES_TIMEOUT_MS;          /**Set the timeout for the upstream NES service in milliseconds*/

    /**
     * @brief The port number for the upstream EVM service.
     */
    int EVMPortNumber = 8002;
    /**
     * @brief The host of the upstream EVM service.
     */
    std::string EVMHost = "localhost";
    /**
     * @brief The timeout for the upstream EVM service in milliseconds.
     */
    int EVMTimeout_ms = 10000;

    /**
     * @brief Sets the port number to be used by the internal Neuroglancer service.
     */
    int NeuroglancerPort = 7998; /**Set the port number to be used by the internal neuroglancer port*/

    std::string PublicHostDomain = "api.braingenix.org"; /**Set the name users will use to connect to your public facing instance (if public)*/

};



}; // Close Namespace Config
}; // Close Namespace API
}; // Close Namespace BG