#include <Util/RestbedHelpers.h>


namespace BG {
namespace API {
namespace Util {


void SendCode(restbed::Session* _Session, int _Code) {
    std::string Body = std::string("{\"StatusCode\":") + std::to_string(_Code) + "}";
    _Session->close(restbed::OK, Body,
      {
        {"Content-Length", std::to_string(Body.size())},
        {"Content-Type", "application/json"}
      }
    );
}


void SendJSON(restbed::Session* _Session, nlohmann::json* _Response) {
    // Return Response String As JSON
    std::string Body = _Response->dump();
    _Session->close(restbed::OK, Body,
      {
        {"Content-Length", std::to_string(Body.size())},
        {"Content-Type", "application/json"}
      }
    );
}

void SendStringifiedJSON(restbed::Session* _Session, const std::string & _Response) {
    // Return Response that was already stringified JSON
    _Session->close(restbed::OK, _Response,
      {
        {"Content-Length", std::to_string(_Response.size())},
        {"Content-Type", "application/json"}
      }
    );
}

bool IsAuthorized(const restbed::Request* _Request) {

  // Check that the AuthKey is present
  if (!_Request->has_query_parameter("AuthKey")) {
      return false;
  }

  // Now Check Token
  std::string Token = _Request->get_query_parameter("AuthKey", "");
  if (Token == "MyVerySecureToken") {
    return true;
  } else {
    return false;
  }

}


void SetVec3(nlohmann::json* _Target, std::string _Input, std::string _Prefix, std::string _Units) {
    nlohmann::json Vec3 = nlohmann::json::parse(_Input);
    (*_Target)[_Prefix + "X_" + _Units] = Vec3[0].template get<float>();
    (*_Target)[_Prefix + "Y_" + _Units] = Vec3[1].template get<float>();
    (*_Target)[_Prefix + "Z_" + _Units] = Vec3[2].template get<float>();
}

void SetVec3(float* _Vector, std::string _Input) {
    nlohmann::json Vec3 = nlohmann::json::parse(_Input);
    _Vector[0] = Vec3[0].template get<float>();
    _Vector[1] = Vec3[1].template get<float>();
    _Vector[2] = Vec3[2].template get<float>();
}


}; // Close Namespace Util
}; // Close Namespace API
}; // Close Namespace BG