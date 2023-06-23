#include <Config/ConfigurationManager.h>


BG::API::Config::Manager(int _NumArgs, char** _Args) {

    // Parse Arguments
    BG::API::Config::ArgumentParser Parser(Config_, _NumArgs, _Args);
    Parser.~ArgumentParser();



}

BG::API::Config::~Manager() {
    
}