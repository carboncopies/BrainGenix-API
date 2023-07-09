#include <Logger/LoggingSystem.h>



namespace BG {
namespace Shared {
namespace Logger {


Logger::Logger(Config _Config) {
    Config_ = _Config;

    try
    {
        logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");
    }
    catch (const spdlog::spdlog_ex &ex)
    {
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }

    console = spdlog::stdout_color_mt("console");
    err_logger = spdlog::stderr_color_mt("stderr");
    spdlog::get("console")->info("loggers can be retrieved from a global registry using the spdlog::get(logger_name)");

    spdlog::info("Test");

}

Logger::~Logger() {

}

bool Logger::Log(std::string _Message, int _LogLevel){

std:cout << "Class method to set log severity" << std::endl;

}

}; // Close Namespace Logger
}; // Close Namespace Shared
}; // Close Namespace BG