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
 * @brief This struct is passed into the callback for each of the resource handlers,
 * and provides access to shared resources (like the db connector).
 * 
 * @details This object must be thread safe, as multiple server threads may access this at any time!
 * 
 */
struct Server {

    std::atomic<int> TotalQueries; /**Test attribute that keeps track of total number of hits the api gets*/

};


}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG