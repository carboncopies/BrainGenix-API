#include <Config/ArgumentParser.h>


namespace BG {
namespace API {
namespace Config {

ArgumentParser::ArgumentParser(BG::Common::Logger::LoggingSystem* _Logger, Config& _Config, int _NumArguments, char** _Args) {

    Logger_ = _Logger;
    // Declare Command Line Options
    boost::program_options::options_description Generic("CLI Options");
    Generic.add_options()
        ("Help,h", "Show Help Message")
        ("Version,v", "Print BG-API Server Version")
        ("CompileTimeStamp,q", "Print BG-API Compile Date Timestamp")
        ("CompilePlatformInfo,w", "Print BG-API Compile Platform Information")
        ("Port, p", boost::program_options::value<int>(&_Config.PortNumber)->default_value(CONFIG_DEFAULT_PORT_NUMBER), "Port Number Of The Service")
        ("Host, o", boost::program_options::value<std::string>(&_Config.Host)->default_value(CONFIG_DEFAULT_HOST), "Host That The Service Binds To")
        ("Config,c", boost::program_options::value<std::string>(&_Config.ConfigFilePaths[0])->default_value(CONFIG_DEFAULT_CFG_FILE_PATH1), "Path To Config File")
        ;
    
    boost::program_options::options_description CommandLineOptions;
    CommandLineOptions.add(Generic);

    // Now, Parse The Options
    boost::program_options::variables_map ArgMap;
    store(boost::program_options::command_line_parser(_NumArguments, _Args).options(CommandLineOptions).run(), ArgMap);
    notify(ArgMap);


    // Print Arguments
    if (ArgMap.count("Help")) {
        std::stringstream ss;
        ss << Generic;
        Logger_->Log(ss.str()+'\n',1);
        exit(0);
    }
    if (ArgMap.count("Version")) {
        Logger_->Log("Version: " VERSION +'\n',1);
        exit(0);
    }
    if (ArgMap.count("CompileTimeStamp")) {
        Logger_->Log("Compile Time Stamp: " COMPILE_TIME_STAMP +'\n',1);
        exit(0);
    }
    if (ArgMap.count("CompilePlatformInfo")) {
        std::string Info = "Compile Platform Information:\n    ";
        Info += std::string("Target OS: ") + TARGET_OS_NAME + "\n    ";
        Info += std::string("Target OS Version: ") + TARGET_OS_VERSION + "\n    ";
        Info += std::string("Target Arch: ") + TARGET_PROCESSOR + "\n    ";
        Info += std::string("Host OS: ") + HOST_OS_NAME + "\n    ";
        Info += std::string("Host OS Version: ") + HOST_OS_VERSION + "\n    ";
        Info += std::string("Host Arch: ") + HOST_PROCESSOR + "\n";
        Logger_->Log(Info,1);
        exit(0);
    }
}


ArgumentParser::~ArgumentParser() {

}


}; // Close Namespace Config
}; // Close Namespace API
}; // Close Namespace BG