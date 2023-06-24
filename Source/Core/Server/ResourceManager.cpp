#include <Server/ResourceManager.h>


namespace BG {
namespace API {
namespace Server {


ResourceManager::ResourceManager(restbed::Service &_Service, Config::Config &_Config) {

    // Setup Our Server Struct
    Server_.TotalQueries = 0;

    // Create Resources


}


ResourceManager::~ResourceManager() {

}



}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG