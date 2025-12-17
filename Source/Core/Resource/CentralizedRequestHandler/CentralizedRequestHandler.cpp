//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file provides implementation for centralized request handling.
    Additional Notes: None
    Date Created: 2025-12-16
*/

// Standard Libraries (BG convention: use <> instead of "")
#include <string>
#include <algorithm>
#include <cctype>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <nlohmann/json.hpp>

// Internal Libraries (BG convention: use <> instead of "")
#include <Resource/CentralizedRequestHandler/CentralizedRequestHandler.h>
#include <Util/RPCHelpers.h>

std::string CentralizedRequestHandler::ToLower(const std::string& _Input) {
    // Converting the input to lower case for case-insensitivity
    std::string result = _Input;
    std::transform(
        result.begin(), 
        result.end(), 
        result.begin(),
        [](unsigned char c) {
            return std::tolower(c);
        }
    );

    return result;
}

std::string CentralizedRequestHandler::RouteToBackendService(
    Server* _Server, 
    BG::Common::Logger::LoggingSystem* _Logger, 
    std::string _TargetService, 
    std::string _RPCQuery, 
    std::string _QueryContent) {
    
    std::string UpstreamResponseStr = "";
    bool Status = false;
    
    std::string targetServiceLower = CentralizedRequestHandler::ToLower(_TargetService);
    std::cout << targetServiceLower;
    
    // TODO: Implement routing logic
    return UpstreamResponseStr;
}

std::string CentralizedRequestHandler::handleRequestNES(std::string _QueryContent) {
    // TODO: Implement NES-specific request handling logic
    // This could include validation, transformation, etc.
    return _QueryContent;
}

std::string CentralizedRequestHandler::handleRequestVSDA(std::string _QueryContent) {
    // TODO: Implement VSDA-specific request handling logic
    return _QueryContent;
}

std::string CentralizedRequestHandler::handleRequestEVM(std::string _QueryContent) {
    // TODO: Implement EVM-specific request handling logic
    return _QueryContent;
}