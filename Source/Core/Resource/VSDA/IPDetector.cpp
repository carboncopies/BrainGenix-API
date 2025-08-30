#include "IPDetector.h"
#include <iostream>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
    #include <iphlpapi.h>
    #pragma comment(lib, "iphlpapi.lib")
#else
    #include <ifaddrs.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
#endif

namespace IPUtils {
    std::vector<std::string> GetLocalIPs() {
        std::vector<std::string> ips;
        
#ifdef _WIN32
        PIP_ADAPTER_ADDRESSES adapterAddresses = NULL;
        ULONG outBufLen = 0;
        DWORD dwRetVal = 0;
        
        // Get the required buffer size
        dwRetVal = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, NULL, &outBufLen);
        if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
            adapterAddresses = (PIP_ADAPTER_ADDRESSES)malloc(outBufLen);
            dwRetVal = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, adapterAddresses, &outBufLen);
        }
        
        if (dwRetVal == NO_ERROR) {
            PIP_ADAPTER_ADDRESSES adapter = adapterAddresses;
            while (adapter) {
                PIP_ADAPTER_UNICAST_ADDRESS unicastAddress = adapter->FirstUnicastAddress;
                while (unicastAddress) {
                    if (unicastAddress->Address.lpSockaddr->sa_family == AF_INET) {
                        sockaddr_in* sa_in = (sockaddr_in*)unicastAddress->Address.lpSockaddr;
                        char ipStr[INET_ADDRSTRLEN];
                        inet_ntop(AF_INET, &(sa_in->sin_addr), ipStr, INET_ADDRSTRLEN);
                        
                        // Skip localhost and invalid addresses
                        std::string ip(ipStr);
                        if (ip != "127.0.0.1" && ip != "0.0.0.0") {
                            ips.push_back(ip);
                        }
                    }
                    unicastAddress = unicastAddress->Next;
                }
                adapter = adapter->Next;
            }
        }
        
        if (adapterAddresses) free(adapterAddresses);
#else
        struct ifaddrs *ifaddr, *ifa;
        if (getifaddrs(&ifaddr) == -1) {
            return ips;
        }
        
        for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr == NULL) continue;
            
            if (ifa->ifa_addr->sa_family == AF_INET) { // IPv4
                struct sockaddr_in* sa = (struct sockaddr_in*)ifa->ifa_addr;
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(sa->sin_addr), ip, INET_ADDRSTRLEN);
                
                // Skip localhost and docker interfaces
                std::string ipStr(ip);
                std::string ifName(ifa->ifa_name);
                if (ipStr != "127.0.0.1" && ifName.find("docker") == std::string::npos && 
                    ifName.find("lo") == std::string::npos) {
                    ips.push_back(ipStr);
                }
            }
        }
        
        freeifaddrs(ifaddr);
#endif
        
        return ips;
    }
    
    std::string GetPrimaryIP() {
        auto ips = GetLocalIPs();
        if (!ips.empty()) {
            return ips[0];
        }
        return "127.0.0.1";
    }
}