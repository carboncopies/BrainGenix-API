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

// Internal Libraries (BG convention: use <> instead of "")


namespace BG {
namespace API {
namespace Server {

/**
 * @brief Enum showing system state codes.
 * 
 */
enum ServiceStatus {
    SERVICE_HEALTHY,
    SERVICE_DEGRADED,
    SERVICE_FAILED,
    SERVICE_NOT_CONFIGURED
};



/**
 * @brief This struct is passed into the callback for each of the resource handlers,
 * and provides access to shared resources (like the db connector).
 * 
 * @details This object must be thread safe, as multiple server threads may access this at any time!
 * 
 */
struct Server {

    std::atomic<int> TotalQueries = 0; /**Test attribute that keeps track of total number of hits the api gets*/
    std::atomic<ServiceStatus> APIState = SERVICE_HEALTHY; /**Enum showing the overall system status*/
    std::atomic<ServiceStatus> NESState = SERVICE_NOT_CONFIGURED; /**Enum showing the NES system status*/

};


}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG