//=================================================================//
// This file is part of the BrainGenix-API Neuron Emulation System //
//=================================================================//

/*
    Description: This file provides RPC management functionality for the system.
    Additional Notes: None
    Date Created: 2024-02-13
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <iostream>
#include <memory>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <nlohmann/json.hpp>


// Internal Libraries (BG convention: use <> instead of "")
#include <BG/Common/Logger/Logger.h>

// #include <Simulator/Structs/Simulation.h>

// #include <RPC/ManagerTaskData.h>
#include <RPC/APIStatusCode.h>


namespace BG {
namespace API {
namespace API {

/**
 * @class HandlerData
 * @brief A class that encapsulates data and utility functions for handling RPC requests.
 *
 * The HandlerData class provides a structured way to manage and process
 * JSON-based RPC requests. It includes functionality for error handling,
 * parameter extraction, and response generation. This class is designed
 * to work with the BrainGenix API and integrates with the logging system.
 *
 * @details
 * - The class stores the JSON request, route path, and status code.
 * - It provides utility functions to extract parameters from JSON requests.
 * - It supports generating various types of responses, including error responses.
 * - The class is designed to be extensible and integrates with other components
 *   such as simulations and logging systems.
 *
 * @note Some methods and members are commented out, indicating potential
 *       future extensions or dependencies on other components.
 *
 * @warning Ensure that the JSON request and response handling is done carefully
 *          to avoid unintended modifications or errors.
 */
class HandlerData {

protected:
    /**
     * @brief Pointer to the instance of the logging system.
     */
    BG::Common::Logger::LoggingSystem* Logger_ = nullptr;

    /**
     * @brief The JSON request string to be processed.
     */
    std::string JSONRequestStr;

    /**
     * @brief Path that is this route.
     */
    std::string RoutePath_;

    /**
     * @brief Parsed JSON object from the request string.
     */
    nlohmann::json RequestJSON;

    /**
     * @brief Status code for the handler.
     */
    BGStatusCode Status = BGStatusSuccess;

    /**
     * @brief Simulation ID associated with the request.
     */
    int SimulationID = -1;

public:
    /**
     * @brief Constructs a HandlerData object with the given JSON request, logger, and route path.
     *
     * @param _JSONRequest The JSON request string to be processed.
     * @param _Logger Pointer to the logging system instance.
     * @param _RoutePath The route path associated with this handler.
     */
    HandlerData(const std::string& _JSONRequest, BG::Common::Logger::LoggingSystem* _Logger, std::string _RoutePath);

    /**
     * @brief Checks if the handler encountered an error.
     * 
     * @return true if there is an error, false otherwise.
     */
    bool HasError() const;

    /**
     * @brief Retrieves the current status code of the handler.
     * 
     * @return BGStatusCode The current status code.
     */
    BGStatusCode GetStatus() const;

    /**
     * @brief Generates a response and optionally stores the request.
     * 
     * @param ResponseJSON The JSON object to be sent as a response.
     * @param store Whether to store the request (default is true).
     * @return std::string The generated response string.
     */
    std::string ResponseAndStoreRequest(nlohmann::json& ResponseJSON, bool store = true);

    /**
     * @brief Generates an error response with a given status code.
     * 
     * @param _Status The integer status code.
     * @return std::string The generated error response string.
     */
    std::string ErrResponse(int _Status);

    /**
     * @brief Generates an error response with a given status code.
     * 
     * @param _Status The BGStatusCode status code.
     * @return std::string The generated error response string.
     */
    std::string ErrResponse(BGStatusCode _Status);

    /**
     * @brief Generates a generic error response.
     * 
     * @return std::string The generated error response string.
     */
    std::string ErrResponse();

    /**
     * @brief Generates a response with an ID field.
     * 
     * @param IDName The name of the ID field.
     * @param IDValue The integer value of the ID.
     * @return std::string The generated response string.
     */
    std::string ResponseWithID(const std::string& IDName, int IDValue);

    /**
     * @brief Generates a response with an ID field.
     * 
     * @param IDName The name of the ID field.
     * @param IDValue The string value of the ID.
     * @return std::string The generated response string.
     */
    std::string ResponseWithID(const std::string& IDName, const std::string& IDValue);

    /**
     * @brief Generates a response with a key-value pair.
     * 
     * @param _Key The key of the response field.
     * @param _Value The value of the response field.
     * @return std::string The generated response string.
     */
    std::string StringResponse(std::string _Key, std::string _Value);

    /**
     * @brief Retrieves the parsed JSON request object.
     * 
     * @return const nlohmann::json& The parsed JSON request object.
     */
    const nlohmann::json& ReqJSON() const;

    /**
     * @brief Finds a parameter in the given JSON object.
     * 
     * @param ParName The name of the parameter to find.
     * @param Iterator An iterator pointing to the parameter if found.
     * @param _JSON The JSON object to search in.
     * @return true if the parameter is found, false otherwise.
     */
    bool FindPar(const std::string& ParName, nlohmann::json::iterator& Iterator, nlohmann::json& _JSON);

    /**
     * @brief Finds a parameter in the request JSON object.
     * 
     * @param ParName The name of the parameter to find.
     * @param Iterator An iterator pointing to the parameter if found.
     * @return true if the parameter is found, false otherwise.
     */
    bool FindPar(const std::string& ParName, nlohmann::json::iterator& Iterator);

    /**
     * @brief Retrieves a boolean parameter from the given JSON object.
     * 
     * @param ParName The name of the parameter to retrieve.
     * @param Value The retrieved boolean value.
     * @param _JSON The JSON object to search in.
     * @return true if the parameter is successfully retrieved, false otherwise.
     */
    bool GetParBool(const std::string& ParName, bool& Value, nlohmann::json& _JSON);

    /**
     * @brief Retrieves a boolean parameter from the request JSON object.
     * 
     * @param ParName The name of the parameter to retrieve.
     * @param Value The retrieved boolean value.
     * @return true if the parameter is successfully retrieved, false otherwise.
     */
    bool GetParBool(const std::string& ParName, bool& Value);

    /**
     * @brief Retrieves an integer parameter from the given JSON object.
     * 
     * @param ParName The name of the parameter to retrieve.
     * @param Value The retrieved integer value.
     * @param _JSON The JSON object to search in.
     * @return true if the parameter is successfully retrieved, false otherwise.
     */
    bool GetParInt(const std::string& ParName, int& Value, nlohmann::json& _JSON);

    /**
     * @brief Retrieves an integer parameter from the request JSON object.
     * 
     * @param ParName The name of the parameter to retrieve.
     * @param Value The retrieved integer value.
     * @return true if the parameter is successfully retrieved, false otherwise.
     */
    bool GetParInt(const std::string& ParName, int& Value);

    /**
     * @brief Retrieves a float parameter from the given JSON object.
     * 
     * @param ParName The name of the parameter to retrieve.
     * @param Value The retrieved float value.
     * @param _JSON The JSON object to search in.
     * @return true if the parameter is successfully retrieved, false otherwise.
     */
    bool GetParFloat(const std::string& ParName, float& Value, nlohmann::json& _JSON);

    /**
     * @brief Retrieves a float parameter from the request JSON object.
     * 
     * @param ParName The name of the parameter to retrieve.
     * @param Value The retrieved float value.
     * @return true if the parameter is successfully retrieved, false otherwise.
     */
    bool GetParFloat(const std::string& ParName, float& Value);

    /**
     * @brief Retrieves a string parameter from the given JSON object.
     * 
     * @param ParName The name of the parameter to retrieve.
     * @param Value The retrieved string value.
     * @param _JSON The JSON object to search in.
     * @return true if the parameter is successfully retrieved, false otherwise.
     */
    bool GetParString(const std::string& ParName, std::string& Value, nlohmann::json& _JSON);

    /**
     * @brief Retrieves a string parameter from the request JSON object.
     * 
     * @param ParName The name of the parameter to retrieve.
     * @param Value The retrieved string value.
     * @return true if the parameter is successfully retrieved, false otherwise.
     */
    bool GetParString(const std::string& ParName, std::string& Value);

    /**
     * @brief Retrieves a vector of integers from the given JSON object.
     * 
     * @param ParName The name of the parameter to retrieve.
     * @param Value The retrieved vector of integers.
     * @param _JSON The JSON object to search in.
     * @return true if the parameter is successfully retrieved, false otherwise.
     */
    bool GetParVecInt(const std::string& ParName, std::vector<int>& Value, nlohmann::json& _JSON);

    /**
     * @brief Retrieves a vector of integers from the request JSON object.
     * 
     * @param ParName The name of the parameter to retrieve.
     * @param Value The retrieved vector of integers.
     * @return true if the parameter is successfully retrieved, false otherwise.
     */
    bool GetParVecInt(const std::string& ParName, std::vector<int>& Value);

    /**
     * @brief Retrieves a vector of floats from the given JSON object.
     * 
     * @param ParName The name of the parameter to retrieve.
     * @param Value The retrieved vector of floats.
     * @param _JSON The JSON object to search in.
     * @return true if the parameter is successfully retrieved, false otherwise.
     */
    bool GetParVecFloat(const std::string& ParName, std::vector<float>& Value, nlohmann::json& _JSON);

    /**
     * @brief Retrieves a vector of floats from the request JSON object.
     * 
     * @param ParName The name of the parameter to retrieve.
     * @param Value The retrieved vector of floats.
     * @return true if the parameter is successfully retrieved, false otherwise.
     */
    bool GetParVecFloat(const std::string& ParName, std::vector<float>& Value);


    // bool GetParVec3FromJSON(const std::string& ParName, Simulator::Geometries::Vec3D& Value, nlohmann::json& _JSON, const std::string& Units = "um");

    // bool GetParVec3(const std::string& ParName, Simulator::Geometries::Vec3D& Value, const std::string& Units = "um");

};


}; // Close Namespace API
}; // Close Namespace API
}; // Close Namespace BG