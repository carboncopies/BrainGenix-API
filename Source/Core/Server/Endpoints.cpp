#include <Server/Endpoints.h>

namespace BG {
namespace API {
namespace Server {

EndpointManager::EndpointManager(BG::Common::Logger::LoggingSystem* _Logger) {
    Logger_ = _Logger;
}

EndpointManager::~EndpointManager() {

}

void EndpointManager::AddRoutes(restbed::Service &_Service, Server &_Server) {


    // Add Defualt Routes (not found/forbidden, etc.)
    NotFound_ = std::make_shared<Resource::NotFound::Route>(&_Server, _Service);
    Forbidden_ = std::make_shared<Resource::Forbidden::Route>(&_Server, _Service);

    // Add Routes
    Root_ = std::make_shared<Resource::Root::Route>(&_Server, _Service);
    Hello_ = std::make_shared<Resource::Hello::Route>(&_Server, _Service);
    Diagnostic_Version_ = std::make_shared<Resource::Diagnostic::Version::Route>(&_Server, _Service);
    Diagnostic_Status_ = std::make_shared<Resource::Diagnostic::Status::Route>(&_Server, _Service);

    Auth_GetToken_ = std::make_shared<Resource::Auth::GetToken::Route>(&_Server, _Service);

    NES_ = std::make_shared<Resource::NES::Route>(Logger_, &_Server, _Service);
    EVM_ = std::make_shared<Resource::EVM::Route>(Logger_, &_Server, _Service);


    NES_Visualizer_RenderImage_ = std::make_shared<Resource::NES::Visualizer::RenderImage::Route>(Logger_, &_Server, _Service);


}

}; // namespace Server
}; // namespace API
}; // namespace BG
