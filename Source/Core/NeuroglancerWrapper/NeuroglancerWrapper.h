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


struct WorkOrder {

    bool IsComplete_ = false; /**Defaults to work is not yet done*/
    std::string DatasetURI_; /**URI to tell neuroglancer to load*/
    std::string DatasetSegmentationURI_; /**Optional URI to tell neuroglancer to load*/
    std::string GeneratedURL_; /**URL that has been generated (only once the system is done)*/

};

/**
 * @brief This class manages the server code. It will initialize restbed with the given config and setup the routes.
 * 
 * @details Within this class, are subclasses each managing different aspects of the restbed service.
 * There will be a database connector class that will handle user authentication checks. Additionally, there will be a
 * route manager class that adds the routes as well as generating the json payload responses.
 */
class NeuroglancerWrapper {

private:

    BG::Common::Logger::LoggingSystem* Logger_; /**Pointer to logger instance*/

    Config::Config* Config_; /**Instance of config pointer*/
    // std::vector<WorkOrder> WorkOrders_; /**List of work orders for the neuroglancer thread to generate URIs for*/
    // std::mutex WorkOrderLock_; /**Mutex to keep the vector thread safe*/
    // std::unique_ptr<pybind11::scoped_interpreter> Guard_; /**Scoped interpreter used to embed the python interpreter into this service*/
    // std::unique_ptr<pybind11::object> Scope_; /**Instance of this python interpreter's scope*/

    // std::unique_ptr<std::thread> ThisThread_; /**Thread that keeps the background python neuroglancer service running*/
    // std::atomic_bool ThreadRun_; /**Tells the thread if it should still run or not*/

    void KeepAliveThread(); /**Method that serves as the thread's main entry point*/

public:

    /**
     * @brief Construct a new Controller object
     * The controller will setup the server based on the provided config pointer's data.
     * 
     * @param _Config 
     */
    NeuroglancerWrapper(Config::Config& _Config, BG::Common::Logger::LoggingSystem* _Logger);


    /**
     * @brief Destroy the Controller object
     * 
     */
    ~NeuroglancerWrapper();


    /**
     * @brief Generates a neuroglancer URL for the target dataset.
     * 
     * @param _DatasetURI 
     * @return std::string 
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