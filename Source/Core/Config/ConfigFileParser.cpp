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
        ("Network.RPCCallback.Port", boost::program_options::value<int>(&_Config.RPCCallbackPort)->default_value(7999), "Port Number Of The RPC Service")
        ("Network.RPCCallback.Host", boost::program_options::value<std::string>(&_Config.RPCCallbackHost)->default_value("0.0.0.0"), "Host That The RPC Service Binds To")
        ("Network.NES.Port", boost::program_options::value<int>(&LocalCfg.NESPortNumber)->default_value(CONFIG_DEFAULT_NES_PORT_NUMBER), "NES Service Port Number")
        ("Network.NES.Host", boost::program_options::value<std::string>(&LocalCfg.NESHost)->default_value(CONFIG_DEFAULT_NES_HOST), "NES Host To Connect To")
        ("Network.NES.Timeout_ms", boost::program_options::value<int>(&LocalCfg.NESTimeout_ms)->default_value(CONFIG_DEFAULT_NES_TIMEOUT_MS), "NES Host To Connect To")        
        ("Network.Service.UseHTTPS", boost::program_options::value<bool>(&LocalCfg.UseHTTPS)->default_value(CONFIG_DEFAULT_USE_HTTPS), "Enable or Disable HTTPS Support")
        ("Network.Service.KeyFilePath", boost::program_options::value<std::string>(&LocalCfg.KeyFilePath)->default_value(""), "Optional Keyfile Path, Required When Using HTTPS")
        ("Network.Service.CrtFilePath", boost::program_options::value<std::string>(&LocalCfg.CrtFilePath)->default_value(""), "Optional Certfile Path, Required When Using HTTPS")
        ("Network.EVM.Port", boost::program_options::value<int>(&_Config.EVMPortNumber)->default_value(8002), "EVM Port Number")
        ("Network.EVM.Host", boost::program_options::value<std::string>(&_Config.EVMHost)->default_value("localhost"), "EVM Host")
        ("Network.EVM.Timeout_ms", boost::program_options::value<int>(&_Config.EVMTimeout_ms)->default_value(5000), "EVM Timeout")
        ("Network.Neuroglancer.Port", boost::program_options::value<int>(&_Config.NeuroglancerPort)->default_value(7998), "Port to use for neuroglancer")
        ("Network.Service.URL", boost::program_options::value<std::string>(&_Config.PublicHostDomain)->default_value("api.example.com"), "Publicly Facing URL to point users towards (for dataset visualization, etc)")
        ;
    
    boost::program_options::options_description ConfigFileOptions;
    ConfigFileOptions.add(FileOptions);

    boost::program_options::variables_map Cfg;


    // Open Configuration File, Parse
    bool HasConfigFile = false;
    for (unsigned int i = 0; i < _Config.ConfigFilePaths.size(); i++) {
        std::ifstream ConfigFileStream(_Config.ConfigFilePaths[i].c_str());
        if (ConfigFileStream) {
            store(parse_config_file(ConfigFileStream, ConfigFileOptions), Cfg);
            notify(Cfg);
            HasConfigFile = true;
            break;
        }
    }
    if (!HasConfigFile) {
        std::cerr<<"[FATAL] Cannot Open Configuration File At `"<<_Config.ConfigFilePaths[0]<<"`.\nAborting.\n";
        exit(1);
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
    if (_Config.UseHTTPS == CONFIG_DEFAULT_USE_HTTPS) {
        _Config.UseHTTPS = LocalCfg.UseHTTPS;
    }
    if (_Config.KeyFilePath == "") {
        _Config.KeyFilePath = LocalCfg.KeyFilePath;
    }
    if (_Config.CrtFilePath == "") {
        _Config.CrtFilePath = LocalCfg.CrtFilePath;
    }
}


ConfigFileParser::~ConfigFileParser() {

}


}; // Close Namespace Config
}; // Close Namespace API
}; // Close Namespace BG