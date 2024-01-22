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

#include <nlohmann/json.hpp>

// Internal Libraries (BG convention: use <> instead of "")
#include <Server/Server.h>

#include <Util/ParamCheck.h>
#include <Util/RestbedHelpers.h>
#include <Util/RPCHelpers.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace Geometry {
namespace Shape {
namespace Sphere {
namespace BulkCreate {


/**
 * @brief Provides the code to handle route for the API.
 * 
 * @param _Server 
 * @param _Session 
 */
class Route {
    
private:

    Server::Server *Server_; /**Copy of the server struct*/

    std::vector<std::string> OptionalParams_; /**List of optional parameters*/
    std::vector<std::string> RequiredParams_; /**List of required parameters*/

public:

    /**
     * @brief Construct a new route object
     * This function registers the routecallback method as a route for the path specified in the descriptor for this class.
     * 
     * @param _Server 
     * @param _Session 
     */
    Route(Server::Server *_Server, restbed::Service &_Service);

    /**
     * @brief Destroy the Route object
     * 
     */
    ~Route();


    /**
     * @brief This is the route callback used.
     * 
     */
    void RouteCallback(const std::shared_ptr<restbed::Session> _Session);

};


}; // Close Namespace
}; // Close Namespace
}; // Close Namespace
}; // Close Namespace
}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG