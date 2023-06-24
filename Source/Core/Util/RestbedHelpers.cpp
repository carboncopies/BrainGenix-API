#include <Util/ParamCheck.h>


namespace BG {
namespace API {
namespace Util {

void SendInvalidParamResponse(restbed::Session* _Session) {
    std::string Body = "{\"StatusCode\":2}";
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
  std::string Token = Request->get_query_parameter("AuthKey", "");
  if (Token == "MyVerySecureToken") {
    return true;
  } else {
    return false;
  }

}

}; // Close Namespace Util
}; // Close Namespace API
}; // Close Namespace BG