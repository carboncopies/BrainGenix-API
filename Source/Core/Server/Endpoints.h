//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file is where all routes are linked into the system
    Additional Notes: None
    Date Created: 2023-06-23
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <string>
#include <memory>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <restbed>

// Internal Libraries (BG convention: use <> instead of "")
#include <Server/Server.h>


#include <Resource/NotFound.h>
#include <Resource/Forbidden.h>

#include <Resource/Hello.h>
#include <Resource/Version.h>
#include <Resource/Status.h>

#include <Resource/Auth/GetToken.h>

#include <Resource/NES/SphereCreate.h>


namespace BG {
namespace API {
namespace Server {

class EndpointManager {

private:

    // Keep a list of resources below, so they stay in scope
    std::shared_ptr<Resource::NotFound::Route> NotFound_;
    std::shared_ptr<Resource::Forbidden::Route> Forbidden_;

    std::shared_ptr<Resource::Hello::Route> Hello_;
    std::shared_ptr<Resource::Diagnostic::Version::Route> Diagnostic_Version_;
    std::shared_ptr<Resource::Diagnostic::Status::Route> Diagnostic_Status_;

    std::shared_ptr<Resource::Auth::GetToken::Route> Auth_GetToken_;

    std::shared_ptr<Resource::NES::Geometry::Shape::Sphere::Create::Route> NES_Geometry_Shape_Sphere_Create_;

public:

    /**
     * @brief Construct a new Endpoint Manager object
     * 
     */
    EndpointManager();

    /**
     * @brief Destroy the Endpoint Manager object
     * 
     */
    ~EndpointManager();


    /**
     * @brief This function is where all routes are added to the service.
     * They're included in this file from the Route directory and then added in the AddRoutes function body.
     * 
     * @param _Server 
     */
    void AddRoutes(restbed::Service &_Service, Server &_Server);


};

}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG