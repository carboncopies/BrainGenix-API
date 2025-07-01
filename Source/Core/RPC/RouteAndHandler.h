//=================================================================//
// This file is part of the BrainGenix-API Neuron Emulation System //
//=================================================================//

/*
    Description: This file provides bgStatusCode.
    Additional Notes: None
    Date Created: 2024-02-06
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <string>
#include <functional>
// #include <memory>
// #include <map>

// Third-Party Libraries (BG convention: use <> instead of "")

// Internal Libraries (BG convention: use <> instead of "")
#include <RPC/ManagerTaskData.h>


namespace BG {
namespace API {
namespace API {



// *** NOTE: By passing JSON objects/components as strings and then having to
//           parse them into JSON objects again, the handlers above are doing
//           a bunch of unnecessary extra work - you can just pass references
//           to components of a JSON object as a JSON object.
//           E.g. call AddBSNeuron(ReqParams) where ReqParams is as obtained
//           in APIRequest() below.

// typedef std::string APIRequest_func_t(const nlohmann::json&);

/**
 * @struct RouteAndHandler
 * @brief A structure that associates a route with its corresponding handler function.
 *
 * This structure is used to map a specific route (endpoint) to a handler function
 * that processes requests for that route.
 */
struct RouteAndHandler {
    /**
     * @brief The route (endpoint) as a string.
     */
    std::string Route_;

    /**
     * @brief A reference to the handler function for the route.
     *
     * The handler function takes a JSON object as input and returns a string response.
     */
    std::function<std::string>& Handler_;
};


}; // Close Namespace API
}; // Close Namespace API
}; // Close Namespace BG
