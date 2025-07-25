//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file manages the RPC client.
    Additional Notes: None
    Date Created: 2023-06-24
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <thread>
#include <atomic>
#include <chrono>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <rpc/client.h>
#include <rpc/rpc_error.h>

// Internal Libraries (BG convention: use <> instead of "")
#include <Version.h>

#include <Config/Config.h>
#include <Server/Server.h>
#include <Util/Types.h>
#include <BG/Common/Logger/Logger.h>


namespace BG {
namespace API {
namespace RPC {

/**
 *
 * 
 * This class manages the RPC client connections to upstream services.
 * It provides methods to query the NES and EVM servers and handles
 * connection health checks and reconnections.
 */
class Manager {

private:
    Config::Config* Config_; /**< Pointer to the configuration instance */
    Server::Server* Server_; /**< Pointer to server struct, this class updates upstream status info */
    BG::Common::Logger::LoggingSystem* Logger_; /**< Pointer to instance of logging system */

    bool RequestThreadsExit_; /**< Used to signal to threads that they should exit */
    std::thread ConnectionManagerNES_; /**< Thread running the NES connection manager */
    std::thread ConnectionManagerEVM_; /**< Thread running the EVM connection manager */

    std::shared_ptr<::rpc::client> NESClient_; /**< Client to upstream NES Service */
    std::shared_ptr<::rpc::client> EVMClient_; /**< Client to upstream EVM Service */
    std::atomic_bool IsNESClientHealthy_; /**< Indicates if the NES service is ready to handle queries */
    std::atomic_bool IsEVMClientHealthy_; /**< Indicates if the EVM service is ready to handle queries */

    /**
     * @brief Attempts to connect to the NES client.
     * 
     * On failure, returns false. If this fails, the client will be set to null.
     * @return true if the connection succeeds.
     * @return false if the connection fails.
     */
    bool ConnectNES();

    /**
     * @brief Attempts to connect to the EVM client.
     * 
     * On failure, returns false. If this fails, the client will be set to null.
     * @return true if the connection succeeds.
     * @return false if the connection fails.
     */
    bool ConnectEVM();

    /**
     * @brief Manages the NES client connection in a separate thread.
     * 
     * This function checks, reconnects, and updates connection data for the NES client.
     */
    void ConnectionManagerNES();

    /**
     * @brief Manages the EVM client connection in a separate thread.
     * 
     * This function checks, reconnects, and updates connection data for the EVM client.
     */
    void ConnectionManagerEVM();

    /**
     * @brief Runs a version check on the NES client.
     * 
     * Updates the server status based on the result. Returns true if everything is good,
     * false if degraded or failed.
     * @return true if the version check succeeds.
     * @return false if the version check fails.
     */
    bool RunVersionCheckNES();

    /**
     * @brief Runs a version check on the EVM client.
     * 
     * Updates the server status based on the result. Returns true if everything is good,
     * false if degraded or failed.
     * @return true if the version check succeeds.
     * @return false if the version check fails.
     */
    bool RunVersionCheckEVM();

    /**
     * @brief Sets callback information for the EVM client.
     */
    void SetEVMCallbackInfo();

    /**
     * @brief Sets callback information for the NES client.
     */
    void SetNESCallbackInfo();

public:
    /**
     * @brief Constructs an RPC client manager.
     * 
     * This manager is used to communicate with upstream services. It reads
     * the configuration passed in to determine connection details.
     * 
     * @param _Logger Pointer to the logging system instance.
     * @param _Config Pointer to the configuration instance.
     * @param _Server Pointer to the server instance.
     */
    Manager(BG::Common::Logger::LoggingSystem* _Logger, Config::Config* _Config, Server::Server* _Server);

    /**
     * @brief Destroys the Manager object.
     */
    ~Manager();

    /**
     * @brief Queries the given route on the NES server with the given JSON.
     * 
     * Populates the result string pointer with the response. Returns false on failure
     * (client error or server issue), otherwise true.
     * 
     * @param _Route The route to query on the NES server.
     * @param _Result Pointer to a string where the response will be stored.
     * @param _ForceQuery If true, forces the query even if the client is marked unhealthy.
     * @return true if the query succeeds.
     * @return false if the query fails.
     */
    bool NESQueryJSON(std::string _Route, std::string* _Result, bool _ForceQuery = false);

    /**
     * @brief Queries the given route on the EVM server with the given JSON.
     * 
     * Populates the result string pointer with the response. Returns false on failure
     * (client error or server issue), otherwise true.
     * 
     * @param _Route The route to query on the EVM server.
     * @param _Result Pointer to a string where the response will be stored.
     * @param _ForceQuery If true, forces the query even if the client is marked unhealthy.
     * @return true if the query succeeds.
     * @return false if the query fails.
     */
    bool EVMQueryJSON(std::string _Route, std::string* _Result, bool _ForceQuery = false);

    /**
     * @brief Queries the given route on the NES server with the given JSON and query string.
     * 
     * Populates the result string pointer with the response. Returns false on failure
     * (client error or server issue), otherwise true.
     * 
     * @param _Route The route to query on the NES server.
     * @param _Query The query string to send to the NES server.
     * @param _Result Pointer to a string where the response will be stored.
     * @param _ForceQuery If true, forces the query even if the client is marked unhealthy.
     * @return true if the query succeeds.
     * @return false if the query fails.
     */
    bool NESQueryJSON(std::string _Route, std::string _Query, std::string* _Result, bool _ForceQuery = false);

    /**
     * @brief Queries the given route on the EVM server with the given JSON and query string.
     * 
     * Populates the result string pointer with the response. Returns false on failure
     * (client error or server issue), otherwise true.
     * 
     * @param _Route The route to query on the EVM server.
     * @param _Query The query string to send to the EVM server.
     * @param _Result Pointer to a string where the response will be stored.
     * @param _ForceQuery If true, forces the query even if the client is marked unhealthy.
     * @return true if the query succeeds.
     * @return false if the query fails.
     */
    bool EVMQueryJSON(std::string _Route, std::string _Query, std::string* _Result, bool _ForceQuery = false);
};

}; // Close Namespace RPC
}; // Close Namespace API
}; // Close Namespace BG
