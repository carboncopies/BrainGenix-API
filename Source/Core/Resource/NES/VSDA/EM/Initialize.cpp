#include <Resource/NES/VSDA/EM/Initialize.h>

namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace VSDA {
namespace EM {
namespace Initialize {

Route::Route(Server::Server *_Server, restbed::Service &_Service) {
    Server_ = _Server;

    // Setup List Of Params
    RequiredParams_.push_back("SimulationID");

    // Setup Callback
    auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

    // Register This Route With Server
    std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
    RouteResource->set_path("/NES/VSDA/EM/Initialize");
    RouteResource->set_method_handler("GET", Callback);
    _Service.publish(RouteResource);
}

Route::~Route() {}

void Route::RouteCallback(const std::shared_ptr<restbed::Session> _Session) {
    const std::shared_ptr<const restbed::Request> Request = _Session->get_request();
    Server_->TotalQueries++;

    // Check Parameters, Early Out If Invalid
    if (!Util::HasRequiredParams(Request.get(), &RequiredParams_)) {
        Util::SendCode(_Session.get(), 2);
        return;
    }

    // Auth Check
    if (!Util::IsAuthorized(Request.get())) {
        Util::SendCode(_Session.get(), 4);
        return;
    }

    // Get Params
    int SimID = Request->get_query_parameter("SimulationID", -1);

    // Upstream Query
    nlohmann::json UpstreamQuery;
    UpstreamQuery["SimulationID"] = SimID;

    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, Server_->IsNESClientHealthy_, "VSDA/EM/Initialize", UpstreamQuery.dump(), &UpstreamResponseStr);
    if (!UpstreamStatus) {
        Util::SendCode(_Session.get(), 3);
        return;
    }
    nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);

    // Send response
    UpstreamResponse["StatusCode"] = 0;

    std::cout << "VSDA EM Initialize Called With Sim ID: " << SimID << std::endl;

    Util::SendJSON(_Session.get(), &UpstreamResponse);
}

}; // namespace Initialize
}; // namespace EM
}; // namespace VSDA
}; // namespace NES

}; // namespace Resource
}; // namespace API
}; // namespace BG
