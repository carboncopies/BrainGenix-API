#include <Logger/LoggingSystem.h>



namespace BG {
namespace Shared {
namespace Logger {


Logger::Logger(Config _Config) {
    Config_ = _Config;

    spdlog::info("Test");

}

Logger::~Logger() {

}


}; // Close Namespace Logger
}; // Close Namespace Shared
}; // Close Namespace BG