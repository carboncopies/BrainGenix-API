#include <Config/ArgumentParser.h>


BG::API::Config::ArgumentParser(Config& _Config, int _NumArguments, char** _Args) {


    // Declare Command Line Options
    boost::program_options::options_description Generic("Generic Options");
    Generic.add_options()
        ("Help,h", "Show Help Message")
        ("Version,v", "Print BG-API Server Version")
        ("CompileTimeStamp,q", "Print BG-API Compile Date Timestamp")
        ("CompilePlatformInfo,w", "Print BG-API Compile Platform Information")
        ("Version,v", "Print BG-API Server Version")
        ("Config,c", boost::program_options::value<std::string>(_Config.ConfigFilePath)->default_value("API.cfg"), "Path To Config File")
        ;
    
    boost::program_options::options_description CommandLineOptions;
    CommandLineOptions.add(Generic);

    // Now, Parse The Options
    boost::program_options::variables_map ArgMap;
    store(boost::program_options::command_line_parser(_NumArguments, _Args).options(CommandLineOptions).run(), ArgMap);
    notify(ArgMap);


    // Print Arguments
    if (ArgMap.count("Help")) {
        std::cout<<Generic<<std::endl;
        exit(0);
    }
    if (ArgMap.count("Version")) {
        std::cout<<"Version: "<<VERSION<<std::endl;
        exit(0);
    }
    if (ArgMap.count("CompileTimeStamp")) {
        std::cout<<"Compile Time Stamp: "<<COMPILE_TIME_STAMP<<std::endl;
        exit(0);
    }
    if (ArgMap.count("CompilePlatformInfo")) {
        std::string Info = "Compile Platform Information:\n    ";
        Info += "Target OS: " + TARGET_OS_NAME + "\n    ";
        Info += "Target OS Version: " + TARGET_OS_VERSION + "\n    ";
        Info += "Target Arch: " + TARGET_PROCESSOR + "\n    ";
        Info += "Host OS: " + HOST_OS_NAME + "\n    ";
        Info += "Host OS Version: " + HOST_OS_VERSION + "\n    ";
        Info += "Host Arch: " + HOST_PROCESSOR + "\n";
        std::cout<<Info;
        exit(0);
    }
}