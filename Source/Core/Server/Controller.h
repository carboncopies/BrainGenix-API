//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file provides the class that manages the web service.
    Additional Notes: None
    Date Created: 2021-11-01
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <regex>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <restbed>

// #include <boost/algorithm/string.hpp>

// Internal Libraries (BG convention: use <> instead of "")
#include <Config/Config.h>
#include <Server/Server.h>

#include <Server/Endpoints.h>

#include <BG/Common/Logger/Logger.h>



namespace BG {
namespace API {
namespace Server {


/**
 * @brief This class manages the server code. It will initialize restbed with the given config and setup the routes.
 * 
 * @details Within this class, are subclasses each managing different aspects of the restbed service.
 * There will be a database connector class that will handle user authentication checks. Additionally, there will be a
 * route manager class that adds the routes as well as generating the json payload responses.
 */
class Controller {

private:

    std::shared_ptr<restbed::Settings> Settings_; /**Shared pointer to settings object, used when starting server. Populated in constructor*/
    restbed::Service HTTPService_; /**Service optionally created when https is on, allows certbot to verify your domain with this*/

    Server Server_; /**Instance of the server struct, used to pass data to callbacks for restbed*/
    // std::string foo_ = "foo";

    std::unique_ptr<EndpointManager> EndpointManager_; /**Class that manages the endpoint resources (Creating them, destroying, etc.)*/

    /**
     * @brief Creates a Restbed settings object from the user configuration struct.
     * 
     * @param _Config Reference to the configuration object containing server settings.
     * @return std::shared_ptr<restbed::Settings> Shared pointer to the configured Restbed settings object.
     */
    std::shared_ptr<restbed::Settings> ConfigureServer(Config::Config &_Config);

    /**
     * @brief Test handler for handling test requests.
     * 
     * @param session Shared pointer to the Restbed session object representing the current client session.
     */
    void TestHandler(const std::shared_ptr<restbed::Session> session);

public:

    restbed::Service Service_; /**< Service created by the constructor, then started with the StartService method. */

    /**
     * @brief Construct a new Controller object.
     * 
     * The controller will set up the server based on the provided configuration data.
     * 
     * @param _Config Reference to the configuration object containing server settings.
     * @param _Logger Pointer to the logging system instance for logging server events.
     */
    Controller(Config::Config& _Config, BG::Common::Logger::LoggingSystem* _Logger);

    /**
     * @brief Destroy the Controller object.
     */
    ~Controller();

    /**
     * @brief Starts the server and blocks until the server stops.
     * 
     * This method assumes the server will run indefinitely unless explicitly stopped.
     */
    void StartService();

    /**
     * @brief Get the Server struct object.
     * 
     * @return Server* Pointer to the server struct instance.
     */
    Server* GetServerStruct();

    /**
     * @brief Hangs the thread until the system is ready to exit.
     */
    void HangUntilExit();

};



}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG