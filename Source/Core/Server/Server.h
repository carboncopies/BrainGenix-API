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
 * and provides access to shared resources (like the db connector).
 * 
 * @details This object must be thread safe, as multiple server threads may access this at any time!
 * 
 */
struct Server {

    std::atomic<int> TotalQueries = 0; /**Test attribute that keeps track of total number of hits the api gets*/
    std::atomic<bgServiceStatus> APIState = SERVICE_HEALTHY; /**Enum showing the overall system status*/
    std::atomic<bgServiceStatus> NESState = SERVICE_CONFIG_ERR; /**Enum showing the NES system status*/
    std::atomic<bgServiceStatus> EVMState = SERVICE_CONFIG_ERR; /**Enum showing the EVM system status*/

    std::shared_ptr<::rpc::client> NESClient; /**Pointer to RPC client service for nes*/
    std::shared_ptr<::rpc::client> EVMClient; /**Pointer to RPC client service for nes*/

    std::atomic_bool* IsNESClientHealthy_; /**Indicates if the upstream service is ready to handle queries. DO NOT QUERY NESCLIENT IF THIS IS FALSE!!!*/
    std::atomic_bool* IsEVMClientHealthy_; /**Indicates if the upstream service is ready to handle queries. DO NOT QUERY NESCLIENT IF THIS IS FALSE!!!*/


};


}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG