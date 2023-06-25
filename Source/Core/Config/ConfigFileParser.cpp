#include <Config/ConfigFileParser.h>


namespace BG {
namespace API {
namespace Config {


ConfigFileParser::ConfigFileParser(Config &_Config) {

    // Create Local Config Object
    Config LocalCfg;

    // Declare Config File Options (note that )
    boost::program_options::options_description FileOptions("Config File Options");
    FileOptions.add_options()
        ("Network.Service.Port", boost::program_options::value<int>(&LocalCfg.PortNumber)->default_value(CONFIG_DEFAULT_PORT_NUMBER), "Port Number Of The Service")
        ("Network.Service.Host", boost::program_options::value<std::string>(&LocalCfg.Host)->default_value(CONFIG_DEFAULT_HOST), "Host That The Service Binds To")
        ("Network.NES.Port", boost::program_options::value<int>(&LocalCfg.NESPortNumber)->default_value(CONFIG_DEFAULT_NES_PORT_NUMBER), "NES Service Port Number")
        ("Network.NES.Host", boost::program_options::value<std::string>(&LocalCfg.NESHost)->default_value(CONFIG_DEFAULT_NES_HOST), "NES Host To Connect To")
        ("Network.NES.Timeout_ms", boost::program_options::value<int>(&LocalCfg.NESTimeout_ms)->default_value(CONFIG_DEFAULT_NES_TIMEOUT_MS), "NES Host To Connect To")
        ;
    
    boost::program_options::options_description ConfigFileOptions;
    ConfigFileOptions.add(FileOptions);

    boost::program_options::variables_map Cfg;


    // Open Configuration File, Parse
    std::ifstream ConfigFileStream(_Config.ConfigFilePath.c_str());
    if (!ConfigFileStream) {
        std::cerr<<"[FATAL] Cannot Open Configuration File At `"<<_Config.ConfigFilePath<<"`.\nAborting.\n";
        exit(1);
    } else {
        store(parse_config_file(ConfigFileStream, ConfigFileOptions), Cfg);
        notify(Cfg);
    }

    // Update Parent's Config As Needed
    if (_Config.PortNumber == CONFIG_DEFAULT_PORT_NUMBER) {
        _Config.PortNumber = LocalCfg.PortNumber;
    }
    if (_Config.Host == CONFIG_DEFAULT_HOST) {
        _Config.Host = LocalCfg.Host;
    }
    if (_Config.NESPortNumber == CONFIG_DEFAULT_NES_PORT_NUMBER) {
        _Config.NESPortNumber = LocalCfg.NESPortNumber;
    }
    if (_Config.NESHost == CONFIG_DEFAULT_NES_HOST) {
        _Config.NESHost = LocalCfg.NESHost;
    }
    if (_Config.NESTimeout_ms == CONFIG_DEFAULT_NES_TIMEOUT_MS) {
        _Config.NESTimeout_ms = LocalCfg.NESTimeout_ms;
    }



}


ConfigFileParser::~ConfigFileParser() {

}


}; // Close Namespace Config
}; // Close Namespace API
}; // Close Namespace BG