#include <Util/RestbedHelpers.h>

#include <iostream>

namespace {

bool ParseVec3Input(const std::string& _Input, float _Values[3]) {
    try {
        nlohmann::json Vec3 = nlohmann::json::parse(_Input);
        if (!Vec3.is_array() || Vec3.size() != 3) {
            std::cout<<"ERR: Vec3 input must be an array with exactly 3 elements\n";
            return false;
        }

        for (size_t i = 0; i < 3; i++) {
            if (!Vec3[i].is_number()) {
                std::cout<<"ERR: Vec3 input element "<<i<<" is not numeric\n";
                return false;
            }
            _Values[i] = Vec3[i].template get<float>();
        }
        return true;
    } catch (const nlohmann::json::exception& e) {
        std::cout<<"ERR: Unable to parse Vec3 input: "<<e.what()<<std::endl;
        return false;
    }
}

} // namespace

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


bool SetVec3(nlohmann::json* _Target, std::string _Input, std::string _Prefix, std::string _Units) {
    if (_Target == nullptr) {
        std::cout<<"ERR: Cannot set Vec3 on null JSON target\n";
        return false;
    }

    float Values[3];
    if (!ParseVec3Input(_Input, Values)) {
        return false;
    }

    (*_Target)[_Prefix + "X_" + _Units] = Values[0];
    (*_Target)[_Prefix + "Y_" + _Units] = Values[1];
    (*_Target)[_Prefix + "Z_" + _Units] = Values[2];
    return true;
}

bool SetVec3(float* _Vector, std::string _Input) {
    if (_Vector == nullptr) {
        std::cout<<"ERR: Cannot set null Vec3 target\n";
        return false;
    }

    float Values[3];
    if (!ParseVec3Input(_Input, Values)) {
        return false;
    }

    _Vector[0] = Values[0];
    _Vector[1] = Values[1];
    _Vector[2] = Values[2];
    return true;
}

