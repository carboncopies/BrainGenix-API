#pragma once

#include <Resource/DTO.h>
#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/outgoing/BufferBody.hpp>
#include <RPC/ClientManager.h>
#include <Util/RPCHelpers.h>
#include <cpp-base64/base64.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <jwt-cpp/jwt.h>
#include <Util/JWTUtil.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController)



// JWT TOKEN verification MACRO
#define CHECK_JWT_OR_UNAUTHORIZED(request, roleOut) \
  const auto& authHeader = request->getHeader("Authorization"); \
  if (authHeader.length() < 8 || authHeader.substr(0, 7) != "Bearer ") { \
    return createResponse(oatpp::web::protocol::http::Status::CODE_401, "Missing or malformed token"); \
  } \
  std::string token = authHeader.substr(7); \
  jwt::decoded_jwt<> decoded; \
  try { decoded = JWTUtil::verifyToken(token); } \
  catch (const std::exception& e) { \
    return createResponse(oatpp::web::protocol::http::Status::CODE_403, "Invalid token: " + std::string(e.what())); \
  } \
  std::string roleOut = JWTUtil::getRole(decoded)

  
class BrainGenixAPIController : public oatpp::web::server::api::ApiController {
  BG::API::Server::Server* Server_;
  BG::API::RPC::Manager* Manager_;
public:
  BrainGenixAPIController(BG::API::Server::Server* _Server, BG::API::RPC::Manager* _Manager, OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : Server_(_Server), Manager_(_Manager), oatpp::web::server::api::ApiController(objectMapper)
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

    //Token Logic 
    std::ifstream file("Config/users.json");
    if(!file.is_open()){
      return createResponse(status::CODE_500 , "unable to open the user information");
    }

    nlohmann::json users;
    file >> users;

    std::string username  = LoginDTO->username;
    std::string password = LoginDTO->password ; 

    if (!users.contains(username) || users[username]["password"] != password) {
        return createResponse(Status::CODE_401, "Invalid credentials");
    }

    std::string role = users[username]["role"];
    std::string token = JWTUtil::generateToken(username, role);


    auto dto = TokenDTO::createShared();
    dto->StatusCode = 0;
    dto->AuthKey = "MyVerySecureToken";

    return createDtoResponse(Status::CODE_200, dto);
  }
  
  ENDPOINT("POST", "/NES", nes, REQUEST(std::shared_ptr<IncomingRequest>, request)) {

    CHECK_JWT_OR_UNAUTHORIZED(request, userRole);

    if (userRole != "admin") {
        return createResponse(Status::CODE_403, "Unauthorized: Admin access only");
    }

    std::string UpstreamResponseStr = "";
    std::string body = request->readBodyToString();
    bool UpstreamStatus = BG::API::Util::NESQueryJSON(Server_->NESClient, Server_->IsNESClientHealthy_, "NES", body, &UpstreamResponseStr);
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

    CHECK_JWT_OR_UNAUTHORIZED(request, userRole);

    
    if (userRole != "admin") {
        return createResponse(Status::CODE_403, "Unauthorized: Admin access only");
    }


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
      Result = BG::API::Util::GetFile(Manager_, FullPath);

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

    CHECK_JWT_OR_UNAUTHORIZED(request, userRole);

    
    if (userRole != "admin") {
        return createResponse(Status::CODE_403, "Unauthorized: Admin access only");
    }


    std::string OverallState = "";
    int SystemState = 3;
    if (Server_->APIState == BG::SERVICE_HEALTHY){
      OverallState = "Healthy";
      SystemState = 0;
    } else if (Server_->APIState == BG::SERVICE_FAILED) {
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
    Response["ServiceStateNES"] = (int)Server_->NESState;
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

  // Also Expose "/.well-known/acme-challenge" for Let's Encrypt to verify from
  ENDPOINT("GET", "/.well-known/acme-challenge/*",  // THIS IS BAD, WE DONT STRIP THINGS, CAUSE IM LAZY!!! FIXME!-This still might be bad - we do strip out '..' but still could be bad.
     acme, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
    std::string Filename = "/" + request->getPathTail();
    
    // Strip Potentially Dangerous '..'
    std::string Pattern = "..";
    std::string::size_type i = Filename.find(Pattern);
    while (i != std::string::npos) {
      std::cout << "Detected '..' In Filename, It's Possible That Someone Is Trying To Do Something Nasty\n";
      Filename.erase(i, Pattern.length());
      i = Filename.find(Pattern, i);
    }

    std::string FinalFilename = "/.well-known/acme-challenge" + Filename;
    std::cout << "[INFO] User Requested File From " << FinalFilename << std::endl;
    std::ifstream Filestream(FinalFilename, std::ifstream::in);
    
    if (Filestream.is_open()) {
      std::string Body = std::string(std::istreambuf_iterator<char>(Filestream), std::istreambuf_iterator<char>());
      
      auto response = createResponse(Status::CODE_200, Body);
      response->putHeader("Content-Type", "text/plain");
      response->putHeader("Content-Length", std::to_string(Body.length())); // don't know if we need to set this manually
      response->putHeader("Access-Control-Allow-Origin", "*");
      return response;
    } else {
      return createResponse(Status::CODE_404, "Not found");;
    }
  }
};

#include OATPP_CODEGEN_END(ApiController) ///< End Codegen