#include <Util/RPCHelpers.h>


namespace BG {
namespace API {
namespace Util {

bool NESQueryJSON(::rpc::client* _Client, std::string _Route, std::string _Query, std::string* _Result) {
    if (!_Client) {
        return false;
    }
    try {
        (*_Result) = _Client->call(_Route.c_str(), _Query).as<std::string>();
    } catch (::rpc::timeout& e) {
        std::cout<<"ERR: NES Connection timed out!\n";
        return false;
    } catch (::rpc::rpc_error& e) {
        std::cout<<"ERR: NES remote returned RPC error\n";
        return false;
    } catch (std::system_error& e) {
        std::cout<<"ERR: Cannot talk to NES host\n";
        return false;
    }
    return true;
}

}; // Close Namespace Util
}; // Close Namespace API
}; // Close Namespace BG