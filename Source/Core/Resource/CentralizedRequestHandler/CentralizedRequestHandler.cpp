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

void CentralizedRequestHandler::LogInitialization(BG::Common::Logger::LoggingSystem* _Logger) {
    if(_Logger) {
        _Logger->Log("[CentralizedRequestHandler] CentralizedRequestHandler initialized and ready to route requests", 5);
    }
}

std::string CentralizedRequestHandler::RouteToBackendService(
    Server* _Server, 
    BG::Common::Logger::LoggingSystem* _Logger, 
    std::string _TargetService, 
    std::string _RPCQuery, 
    std::string _QueryContent) {
    
    std::string UpstreamResponseStr = "";
    bool Status = false;
    
    // Log that CentralizedRequestHandler is routing a request
    if(_Logger) {
        _Logger->Log("[CentralizedRequestHandler] Routing request to service: '" + _TargetService + "' with RPC query: '" + _RPCQuery + "'", 5);
    }
    
    std::string targetServiceLower = CentralizedRequestHandler::ToLower(_TargetService);

    if(targetServiceLower == "nes") {
        UpstreamResponseStr = handleRequestNES(_QueryContent, _RPCQuery, _Logger);
    }
    else if(targetServiceLower == "evm") {
        UpstreamResponseStr = handleRequestEVM(_QueryContent, _RPCQuery, _Logger);
    }
    else if(targetServiceLower == "vsda") {
        UpstreamResponseStr = handleRequestVSDA(_QueryContent, _RPCQuery, _Logger);
    }   
    else {
        // Log error for invalid target service
        if(_Logger) {
            _Logger->Log("[CentralizedRequestHandler] Invalid target service: '" + _TargetService + "' (normalized: '" + targetServiceLower + "'). Supported services: NES, EVM, VSDA", 7);
        }
    }

    // Log successful routing completion
    if(_Logger) {
        _Logger->Log("[CentralizedRequestHandler] Successfully routed request to '" + targetServiceLower + "', response length: " + std::to_string(UpstreamResponseStr.length()) + " bytes", 5);
    }

    return UpstreamResponseStr;
}

std::string CentralizedRequestHandler::handleRequestNES(std::string _QueryContent, std::string _RPCQuery, BG::Common::Logger::LoggingSystem* _Logger) {
    // TODO: Implement NES-specific request handling logic
    // This could include validation, transformation, etc.

    // Log that this service is being pinged
    if(_Logger) {
        _Logger->Log("[CentralizedRequestHandler] NES service is being pinged with RPC query: '" + _RPCQuery + "'", 5);
    }
    
    // For testing purposes we are returning a ping response
    std::string pingResponse = "PING PING";

    return pingResponse;
}

std::string CentralizedRequestHandler::handleRequestVSDA(std::string _QueryContent, std::string _RPCQuery, BG::Common::Logger::LoggingSystem* _Logger) {
    // TODO: Implement VSDA-specific request handling logic
    if(_Logger) {
        _Logger->Log("[CentralizedRequestHandler] VSDA service request with RPC query: '" + _RPCQuery + "'", 5);
    }
    return _QueryContent;
}

std::string CentralizedRequestHandler::handleRequestEVM(std::string _QueryContent, std::string _RPCQuery, BG::Common::Logger::LoggingSystem* _Logger) {
    // TODO: Implement EVM-specific request handling logic
    if(_Logger) {
        _Logger->Log("[CentralizedRequestHandler] EVM service request with RPC query: '" + _RPCQuery + "'", 5);
    }
    return _QueryContent;
}