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

#include <nlohmann/json.hpp>

// Internal Libraries (BG convention: use <> instead of "")


namespace BG {
namespace API {
namespace Util {


/**
 * @brief General handler for sending back status codes.
 * 0 is success (but don't do that here, use the SendJSON helper instead)
 * 1 is general failure (unknown why it failed but something broke)
 * 2 is invalid parameters
 * 3 is downstream gateway unavailable
 * 4 is unauthorized
 * more to be added as needed, check spec for most up to date info.
 * 
 * @param _Session 
 */
void SendCode(restbed::Session* _Session, int _Code);


/**
 * @brief Used to send JSON back to the client. Pass in the session ptr as well as a pointer to your json data.
 * Remember to set your StatusCode!
 * 
 * @param _Session 
 * @param _Response 
 */
void SendJSON(restbed::Session* _Session, nlohmann::json* _Response);



/**
 * @brief Simple auth check, need to fix this later to use the database or something...
 * 
 * @param _Request 
 * @return true 
 * @return false 
 */
bool IsAuthorized(const restbed::Request* _Request);


}; // Close Namespace Util
}; // Close Namespace API
}; // Close Namespace BG