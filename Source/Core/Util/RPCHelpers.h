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
#include <iostream>
#include <memory>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <rpc/client.h>
#include <rpc/rpc_error.h>

#include <nlohmann/json.hpp>
// Internal Libraries (BG convention: use <> instead of "")


namespace BG {
namespace API {
namespace Util {


/**
 * @brief Queries the given route on the NES server with the given json.
 * Populates the result std::string ptr with the response.
 * Returns false on fail (client error/server issue), otherwise true.
 * 
 * @param _Client
 * @param _Route 
 * @param _Query
 * @param _Result 
 * @return true 
 * @return false 
 */
bool NESQueryJSON(std::shared_ptr<::rpc::client> _Client, std::atomic_bool* _IsEVMClientHealthy,std::string _Route, std::string _Query, std::string* _Result);
bool EVMQueryJSON(std::shared_ptr<::rpc::client> _Client, std::atomic_bool* _IsEVMClientHealthy,std::string _Route, std::string _Query, std::string* _Result);


}; // Close Namespace Util
}; // Close Namespace API
}; // Close Namespace BG