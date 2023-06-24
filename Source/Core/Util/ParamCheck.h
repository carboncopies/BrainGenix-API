//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file provides helper utils for validating required parameters.
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
 * @brief Handy little helper function to check if the request has a set list of required parameters.
 * 
 * @param _Request 
 * @param _RequiredParams 
 * @return true 
 * @return false 
 */
bool HasRequiredParams(restbed::Request* _Request, std::vector<std::string>* _RequiredParams);


}; // Close Namespace Util
}; // Close Namespace API
}; // Close Namespace BG