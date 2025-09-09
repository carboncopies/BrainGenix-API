#include <RPC/StaticRoutes.h>




std::string GetAPIVersion() {
    return VERSION; // VERSION is a pre-defined constant or variable.
}

std::string Echo(std::string _Data) {
    return _Data;
}

