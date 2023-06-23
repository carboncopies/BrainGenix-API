#include <Config/ConfigurationManager.h>



namespace BG {
namespace API {
namespace Config {


Manager::Manager(int _NumArgs, char** _Args) {

    // Parse Arguments
    ArgumentParser Parser(Config_, _NumArgs, _Args);
    Parser.~ArgumentParser();



}

Manager::~Manager() {

}


}; // Close Namespace Config
}; // Close Namespace API
}; // Close Namespace BG