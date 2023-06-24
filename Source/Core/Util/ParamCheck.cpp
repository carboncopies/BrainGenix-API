#include <Util/ParamCheck.h>


namespace BG {
namespace API {
namespace Util {

bool HasRequiredParams(const restbed::Request* _Request, std::vector<std::string>* _RequiredParams) {
    for (unsigned int i = 0; i < _RequiredParams->size(); i++) {
        std::string Param = (*_RequiredParams)[i];
        if (!_Request->has_query_parameter(Param)) {
            return false;
        }
    }
    return true;
}


}; // Close Namespace Util
}; // Close Namespace API
}; // Close Namespace BG