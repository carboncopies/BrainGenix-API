//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file provides helper utils for managing restbed requests/responses.
    Additional Notes: None
    Date Created: 2023-06-24
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <string>
#include <vector>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <restbed>

// Internal Libraries (BG convention: use <> instead of "")


namespace BG {
namespace API {
namespace Util {


/**
 * @brief Helper to quickly send back status code 2 failure (invalid parameters passed)
 * 
 * @param _Session 
 */
void SendInvalidParamResponse(restbed::Session* _Session);


}; // Close Namespace Util
}; // Close Namespace API
}; // Close Namespace BG