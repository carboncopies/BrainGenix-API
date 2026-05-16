#include <Config/ConfigurationManager.h>



ConfigManager::ConfigManager(BG::Common::Logger::LoggingSystem* _Logger, int _NumArgs, char** _Args) {

    {
        ArgumentParser ArgParser(_Logger, Config_, _NumArgs, _Args);
    }

    // Now Load Configuration File
    {
        ConfigFileParser ConfigParser(Config_);
    }

}

ConfigManager::~ConfigManager() {

}



Config& ConfigManager::GetConfig() {
    return Config_;
}

