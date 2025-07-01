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
#include <iostream>
#include <functional>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <restbed>

// Internal Libraries (BG convention: use <> instead of "")
#include <Server/Server.h>


namespace BG {
namespace API {
namespace Resource {

namespace Root {


/**
 * @brief Provides the code to handle the / route for the API.
 * 
 * @param _Session The session object representing the current client session.
 */
class Route {
    
private:

    Server::Server *Server_; /**< Pointer to the server instance. */

public:

    /**
     * @brief Construct a new Route object.
     * 
     * This function registers the RouteCallback method as a route for the path specified in the descriptor for this class.
     * 
     * @param _Server Pointer to the server instance.
     * @param _Service Reference to the Restbed service instance.
     */
    Route(Server::Server *_Server, restbed::Service &_Service);

    /**
     * @brief Destroy the Route object.
     */
    ~Route();

    /**
     * @brief Callback function for handling the route.
     * 
     * @param _Session Shared pointer to the Restbed session object.
     */
    void RouteCallback(const std::shared_ptr<restbed::Session> _Session);

};


}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG