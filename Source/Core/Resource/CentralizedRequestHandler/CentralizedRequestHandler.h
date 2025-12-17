//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file provides code used in the API routes.
    Additional Notes: None
    Date Created: 2025-12-16
*/

#pragma once    

// Standard Libraries (BG convention: use <> instead of "")
#include <string>

// Internal Libraries (BG convention: use <> instead of "")
#include <Server/Server.h>
#include <BG/Common/Logger/Logger.h>

class CentralizedRequestHandler {
    public:
        /**
         * @brief Converts a string to lowercase
         * 
         * This is a utility function that converts all characters in a string to lowercase.
         * Useful for case-insensitive string comparisons.
         * 
         * @param _Input The input string to convert to lowercase
         * @return std::string The input string converted to lowercase
         */
        static std::string ToLower(const std::string& _Input); 

    private:
        /**
            * @brief Routes a request from one backend service to another through the API.
            *
            * This method allows backend services (NES, VSDA, EVM, ERS) to communicate with
            * each other by routing requests through the API hub. The API forwards the request
            * to the target service and returns the response.
            * Supported Services: NES, VSDA, EVM, ERS
            *
            * @param _Server Pointer to the Server instance containing client connections.
            * @param _Logger Pointer to the logging system instance.
            * @param _TargetService Name of the API that we want to route the request to.
            * @param _RPCQuery Name of the RPC query to route the request to.
            * @param _QueryContent JSON query content to be sent to the API.
            * @return std::string JSON response string from the target service, or error JSON if routing fails.
            */
        std::string RouteToBackendService(Server* _Server, BG::Common::Logger::LoggingSystem* _Logger, std::string _TargetService, std::string _RPCQuery, std::string _QueryContent);

        /**
            * @brief This function handles the intended functionality of a Request coming from NES
            *
            * This method indicates how to handle requests that are coming from NES, how they should be processed etc.
            *
            * @param _QueryContent The JSON query content which will be processed and then sent back
            */
        std::string handleRequestNES(std::string _QueryContent);

        /**
            * @brief This function handles the intended functionality of a Request coming from VSDA
            * This method indicates how to handle requests that are coming from VSDA, how they should be processed etc.
            *
            * @param _QueryContent The JSON query content which will be processed and then sent back
            */
        std::string handleRequestVSDA(std::string _QueryContent);

        /**
            * @brief This function handles the intended functionality of a Request coming from EVM
            * This method indicates how to handle requests that are coming from EVM, how they should be processed etc.
            *
            * @param _QueryContent The JSON query content which will be processed and then sent back
            */
        std::string handleRequestEVM(std::string _QueryContent);
};

