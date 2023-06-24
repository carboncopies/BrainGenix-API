//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file provides code used in the API routes.
    Additional Notes: None
    Date Created: 2023-06-23
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <string>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <restbed>

// Internal Libraries (BG convention: use <> instead of "")
#include <Server/Server.h>


namespace BG {
namespace API {
namespace Resource {
namespace Route {

/**
 * @brief Provides the code to handle the /Hello route for the API.
 * 
 * @param _Server 
 * @param _Session 
 */
void Hello(Server::Server &_Server, const std::shared_ptr<restbed::Session> _Session);


}; // Close Namespace Route
}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG