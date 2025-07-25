#pragma once

#include <Resource/DTO.h>
#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/outgoing/BufferBody.hpp>
#include <Server/Server.h>
#include <Util/RPCHelpers.h>
#include <cpp-base64/base64.h>
#include <nlohmann/json.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController)

class BrainGenixAPIController : public oatpp::web::server::api::ApiController {
public:
  BrainGenixAPIController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
public:
  ENDPOINT("GET", "/", root) {
    auto dto = MessageDto::createShared();
    dto->statusCode = 200;
    dto->message = "Welcome to the BrainGenix API. For usage, please see our documentation at https://gitlab.braingenix.org/carboncopies/BrainGenix-API";

    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT("GET", "/Hello", hello) {
    auto dto = MessageDto::createShared();
    dto->statusCode = 200;
    dto->message = "https://www.youtube.com/watch?v=dQw4w9WgXcQ\nGet Rickrolled lol.";

    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT("GET", "/Auth/GetToken", token) {
    auto dto = TokenDTO::createShared();
    dto->StatusCode = 0;
    dto->AuthKey = "MyVerySecureToken";

    return createDtoResponse(Status::CODE_200, dto);
  }
  
  ENDPOINT("POST", "/NES", nes, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
      printf("starting response return\n");
      std::string UpstreamResponseStr = "";
      std::string body = request->readBodyToString();
      bool UpstreamStatus = BG::API::Util::NESQueryJSON(g_Server->NESClient, g_Server->IsNESClientHealthy_, "NES", body, &UpstreamResponseStr);
      if (!UpstreamStatus) {
        printf("Upstream status fail\n");  
        return createResponse(Status::CODE_204, "Upstream status fail");
      }

      //printf("%s\n%s\n\n", body.c_str(), UpstreamResponseStr.c_str());

      auto response = createResponse(Status::CODE_200, UpstreamResponseStr);
      response->putHeader("Content-Type", "application/json"); // not sure if this is needed
      return response;
  }

  ENDPOINT("GET", "/Dataset/*", dataset, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
    auto tail = request->getPathTail(); // everything after /Dataset/

    // parse path in tail
    std::vector<std::string> segments;
    std::istringstream ss(*tail);
    std::string segment;
    while (std::getline(ss, segment, '/')) {
      if (!segment.empty()) {
        segments.push_back(segment);
      }
    }

    std::string FullPath = "NeuroglancerDatasets/";

    // could do this with a loop and check size beforehand
    switch (segments.size()) {
      case 2:
        // Path/Level
        FullPath += segments[0] + "/" + segments[1];
        break;
      case 3:
        // Path/Level/File
        FullPath += segments[0] + "/" + segments[1] + "/" + segments[2];
        break;
      case 4:
        // Path/Level/Level2/File
        FullPath += segments[0] + "/" + segments[1] + "/" + segments[2] + "/" + segments[3];
        break;
      default:
        return createResponse(Status::CODE_404, "Invalid path");
    }

    std::string Result;
    try {
      Result = BG::API::Util::GetFile(FullPath);

      if (Result.empty()) {
        return createResponse(Status::CODE_404, "No file found. Path: " + FullPath);
      }
    } catch(std::exception& e) {
      std::cout << e.what() << '\n';
      return createResponse(Status::CODE_404, "Invalid path");
    }

    auto DecodedString = base64_decode(Result, false);
    auto response = createResponse(Status::CODE_200, DecodedString);

    // not sure if we need to do all of this manually
    response->putHeader("Content-Type", "application/octet-stream");
    response->putHeader("Content-Length", std::to_string(DecodedString.length()));
    
    //printf("%s\n%s\n\n", FullPath.c_str(), DecodedString.c_str());

    // not sure why default response doesn't allow this, but we need this for neuroglancer requests because it will get blocked otherwise
    response->putHeader("Access-Control-Allow-Origin", "*"); // legacy code says that this should be done for every endpoint, but this seems to be working fine. need to find out how oatpp handles default headers
    response->putHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    response->putHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");

    return response;
  }

  ENDPOINT("GET", "/Diagnostic/Status", status) {
    // Setup Response
    std::string OverallState = "";
    int SystemState = 3;
    if (g_Server->APIState == BG::SERVICE_HEALTHY){
      OverallState = "Healthy";
      SystemState = 0;
    } else if (g_Server->APIState == BG::SERVICE_FAILED) {
      OverallState = "Failed";
      SystemState = 2;
    } else {
      OverallState = "Degraded";
      SystemState = 1;
    }

    // Build JSON Response
    nlohmann::json Response;
    Response["StatusCode"] = 0;
    Response["SystemState"] = OverallState;
    Response["ServiceStateNES"] = (int)g_Server->NESState;
    Response["ServiceStateAPI"] = SystemState;
    Response["ServiceStateAPIDB"] = 3; // not yet configured
    Response["ServiceStateERS"] = 3; // not yet configured
    Response["ServiceStateSTS"] = 3; // not yet configured

    // Return Response String As JSON
    std::string Body = Response.dump();

    auto response = createResponse(Status::CODE_200, Body);
    response->putHeader("Content-Type", "application/json");
    response->putHeader("Access-Control-Allow-Origin", "*");
    return response;
  }
};

#include OATPP_CODEGEN_END(ApiController) ///< End Codegen