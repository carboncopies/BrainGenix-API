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

}; // Close Namespace Util
}; // Close Namespace API
}; // Close Namespace BG