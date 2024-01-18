#include <Config/ConfigurationManager.h>



namespace BG {
namespace API {
namespace Config {


Manager::Manager(BG::Common::Logger::LoggingSystem* _Logger, int _NumArgs, char** _Args) {

    ArgumentParser ArgParser(_Logger, Config_, _NumArgs, _Args);
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