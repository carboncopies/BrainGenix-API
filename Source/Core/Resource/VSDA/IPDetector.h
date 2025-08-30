#pragma once
#include <string>
#include <vector>

namespace IPUtils {
    std::vector<std::string> GetLocalIPs();
    std::string GetPrimaryIP();
}