#include <Logger/LoggingSystem.h>


namespace BG {
namespace Shared {
namespace Logger {


Logger::Logger(Config _Config) {
    Config_ = _Config;
    
    //Logger_ = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");

    // try
    // {
    // }
    // catch (const spdlog::spdlog_ex &ex)
    // {
        // std::cout << "Log init failed: " << ex.what() << std::endl;
    // }

    // auto console = spdlog::stdout_color_mt("console");
    // auto err_logger = spdlog::stderr_color_mt("stderr");
    // spdlog::get("console")->info("loggers can be retrieved from a global registry using the spdlog::get(logger_name)");

    // spdlog::info("Test");

}

Logger::~Logger() {

}

bool Logger::Log(std::string _Message, int _LogLevel){
    std::cout << "Class method to set log severity" << std::endl;

    if(_LogLevel==0){
        spdlog::debug(_Message);
    }
    else if(_LogLevel==1){
        spdlog::info(_Message);
    }
    else if(_LogLevel==2){
        spdlog::warn(_Message);
    }
    else if(_LogLevel==3){
        spdlog::error(_Message);
    }
    else if(_LogLevel==4){
        spdlog::critical(_Message);
    }

    return true;

}

}; // Close Namespace Logger
}; // Close Namespace Shared
}; // Close Namespace BG