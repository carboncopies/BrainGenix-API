#include <Config/ConfigurationManager.h>



namespace BG {
namespace API {
namespace Config {


Manager::Manager(int _NumArgs, char** _Args) {

    std::unique_ptr<BG::Common::Logger::LoggingSystem> Logger=std::make_unique<BG::Common::Logger::LoggingSystem>();    // Parse Arguments
    ArgumentParser ArgParser(std::move(Logger),Config_, _NumArgs, _Args);
    ArgParser.~ArgumentParser();

    // Now Load Configuration File
    ConfigFileParser ConfigParser(Config_);
    ConfigParser.~ConfigFileParser();

}

Manager::~Manager() {

}



Config& Manager::GetConfig() {
    return Config_;
}


}; // Close Namespace Config
}; // Close Namespace API
}; // Close Namespace BG