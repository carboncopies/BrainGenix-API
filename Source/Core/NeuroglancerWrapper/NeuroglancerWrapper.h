//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file provides the class that manages the web service.
    Additional Notes: None
    Date Created: 2024-04-12
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>
#include <regex>

// Third-Party Libraries (BG convention: use <> instead of "")
// #include <pybind11/pybind11.h>
// #include <pybind11/embed.h>


// Internal Libraries (BG convention: use <> instead of "")
#include <Config/Config.h>

#include <BG/Common/Logger/Logger.h>



namespace BG {
namespace API {

/**
 * @struct WorkOrder
 * @brief Represents a work order for generating a Neuroglancer visualization.
 *
 * This struct contains information about the dataset and its segmentation, as well as
 * the generated URL for visualization. It also tracks whether the work order is complete.
 */
struct WorkOrder {

    /**
     * @brief Indicates whether the work order is complete.
     * 
     * Defaults to `false`, meaning the work is not yet done.
     */
    bool IsComplete_ = false;

    /**
     * @brief URI to tell Neuroglancer to load the dataset.
     */
    std::string DatasetURI_;

    /**
     * @brief Optional URI to tell Neuroglancer to load the dataset segmentation.
     */
    std::string DatasetSegmentationURI_;

    /**
     * @brief URL that has been generated (only once the system is done).
     */
    std::string GeneratedURL_;
};

/**
 * @brief This class manages the server code. It will initialize restbed with the given config and setup the routes.
 * 
 * @details Within this class, are subclasses each managing different aspects of the restbed service.
 * There will be a database connector class that will handle user authentication checks. Additionally, there will be a
 * route manager class that adds the routes as well as generating the json payload responses.
 */
/**
 * @brief Construct a new NeuroglancerWrapper object.
 * 
 * The constructor initializes the NeuroglancerWrapper with the provided configuration and logger instances.
 * 
 * @param _Config Reference to a Config object containing configuration data for the NeuroglancerWrapper.
 * @param _Logger Pointer to a LoggingSystem instance used for logging within the NeuroglancerWrapper.
 */
NeuroglancerWrapper(Config::Config& _Config, BG::Common::Logger::LoggingSystem* _Logger);


    /**
     * @brief Destroy the Controller object
     * 
     */
    ~NeuroglancerWrapper();


    /**
     * @brief Generates a Neuroglancer URL for the target dataset.
     * 
     * @param _DatasetURI The URI of the dataset to be visualized. Defaults to "http://localhost:9000/Example".
     * @param _DatasetSegURI The URI of the dataset segmentation, if applicable. Defaults to an empty string.
     * @return std::string The generated Neuroglancer URL.
     */
    std::string GetNeuroglancerURL(std::string _DatasetURI="http://localhost:9000/Example", std::string _DatasetSegURI="");

    /**
     * @brief Helper function that generates the full link for both the dataset, and then the neuroglancer link, sending it back to the user.
     * 
     * @param _Request 
     * @return std::string 
     */
    std::string GetVisualizerLink(std::string _Request);

};



}; // Close Namespace API
}; // Close Namespace BG