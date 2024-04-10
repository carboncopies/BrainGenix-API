#include <Resource/NES/VSDA/Calcium/Setup.h>

namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace VSDA {
namespace Calcium {
namespace Setup {

Route::Route(Server::Server *_Server, restbed::Service &_Service) {
    Server_ = _Server;

    // Setup List Of Params
    RequiredParams_.push_back("SimulationID");
    RequiredParams_.push_back("PixelResolution_um");
    RequiredParams_.push_back("ImageWidth_px");
    RequiredParams_.push_back("ImageHeight_px");
    RequiredParams_.push_back("SliceThickness_um");
    RequiredParams_.push_back("ScanRegionOverlap_percent");
    RequiredParams_.push_back("MicroscopeFOV_deg");
    RequiredParams_.push_back("NumPixelsPerVoxel_px");
    RequiredParams_.push_back("FlourescingNeuronTags");
    RequiredParams_.push_back("ImagingInterval_ms");

    // Setup Callback
    auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

    // Register This Route With Server
    std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
    RouteResource->set_path("/NES/VSDA/Calcium/Setup");
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
    float PixelResolution_um = Request->get_query_parameter("PixelResolution_um", -1.);
    int ImageWidth_px = Request->get_query_parameter("ImageWidth_px", -1);
    int ImageHeight_px = Request->get_query_parameter("ImageHeight_px", -1);
    float SliceThickness_um = Request->get_query_parameter("SliceThickness_um", -1.);
    float ScanRegionOverlap_percent = Request->get_query_parameter("ScanRegionOverlap_percent", -1.);
    float MicroscopeFOV_deg = Request->get_query_parameter("MicroscopeFOV_deg", -1.);
    int NumPixelsPerVoxel_px = Request->get_query_parameter("NumPixelsPerVoxel_px", -1);
    float ImagingInterval_ms = Request->get_query_parameter("ImagingInterval_ms", -1.);

    std::vector<std::string> FlourescingNeuronTags = nlohmann::json(Request->get_query_parameter("FlourescingNeuronTags", "")).get<std::vector<std::string>>();

    // Upstream Query
    nlohmann::json UpstreamQuery;
    UpstreamQuery["SimulationID"] = SimID;
    UpstreamQuery["PixelResolution_um"] = PixelResolution_um;
    UpstreamQuery["ImageWidth_px"] = ImageWidth_px;
    UpstreamQuery["ImageHeight_px"] = ImageHeight_px;
    UpstreamQuery["SliceThickness_um"] = SliceThickness_um;
    UpstreamQuery["ScanRegionOverlap_percent"] = ScanRegionOverlap_percent;
    UpstreamQuery["MicroscopeFOV_deg"] = MicroscopeFOV_deg;
    UpstreamQuery["NumPixelsPerVoxel_px"] = NumPixelsPerVoxel_px;
    UpstreamQuery["ImagingInterval_ms"] = ImagingInterval_ms;
    UpstreamQuery["FlourescingNeuronTags"] = FlourescingNeuronTags;

    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, Server_->IsNESClientHealthy_, "VSDA/Calcium/Setup", UpstreamQuery.dump(), &UpstreamResponseStr);
    if (!UpstreamStatus) {
        Util::SendCode(_Session.get(), 3);
        return;
    }
    nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);

    // Send Response
    UpstreamResponse["StatusCode"] = 0;

    std::cout << "VSDA Calcium CreateIndicator Called With Sim ID: " << SimID << std::endl;

    Util::SendJSON(_Session.get(), &UpstreamResponse);
}

}; // namespace Setup
}; // namespace Calcium
}; // namespace VSDA
}; // namespace NES

}; // namespace Resource
}; // namespace API
}; // namespace BG
