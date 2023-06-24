#include <Config/ConfigurationManager.h>



namespace BG {
namespace API {
namespace Config {


Manager::Manager(int _NumArgs, char** _Args) {

    // Parse Arguments
    ArgumentParser ArgParser(Config_, _NumArgs, _Args);
    ArgParser.~ArgumentParser();

    // Now Load Configuration File
    ConfigFileParser ConfigParser(Config_);
    ConfigParser.~ConfigFileParser();

}

Manager::~Manager() {

}


}; // Close Namespace Config
}; // Close Namespace API
}; // Close Namespace BG