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


}; // Close Namespace Util
}; // Close Namespace API
}; // Close Namespace BG