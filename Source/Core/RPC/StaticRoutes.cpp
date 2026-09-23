#include <RPC/StaticRoutes.h>




std::string GetAPIVersion() {
    return NES_API_VERSION;
}

std::string Echo(std::string _Data) {
    return _Data;
}

