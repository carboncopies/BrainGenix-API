//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file provides the server shared data struct.
    Additional Notes: None
    Date Created: 2023-06-23
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <string>
#include <atomic>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <rpc/client.h>

// Internal Libraries (BG convention: use <> instead of "")
#include <Util/Types.h>


namespace BG {
namespace API {
namespace Server {





/**
 * @brief This struct is passed into the callback for each of the resource handlers,
 * and provides access to shared resources (like the database connector).
 * 
 * @details This object must be thread-safe, as multiple server threads may access it at any time.
 */
struct Server {

    std::atomic<int> TotalQueries = 0; /**< Tracks the total number of API queries received. */
    std::atomic<bgServiceStatus> APIState = SERVICE_HEALTHY; /**< Enum indicating the overall system status. */
    std::atomic<bgServiceStatus> NESState = SERVICE_CONFIG_ERR; /**< Enum indicating the NES system status. */
    std::atomic<bgServiceStatus> EVMState = SERVICE_CONFIG_ERR; /**< Enum indicating the EVM system status. */

    std::shared_ptr<::rpc::client> NESClient; /**< Shared pointer to the RPC client service for NES. */
    std::shared_ptr<::rpc::client> EVMClient; /**< Shared pointer to the RPC client service for EVM. */

    std::atomic_bool* IsNESClientHealthy_; /**< Indicates if the NES client is ready to handle queries. */
    std::atomic_bool* IsEVMClientHealthy_; /**< Indicates if the EVM client is ready to handle queries. */

};


}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG