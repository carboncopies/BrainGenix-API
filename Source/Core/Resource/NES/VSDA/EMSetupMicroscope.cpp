#include <Resource/NES/VSDA/EMSetupMicroscope.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace VSDA {
namespace EM {
namespace SetupMicroscope {


Route::Route(Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;

  // Setup List Of Params
  RequiredParams_.push_back("SimulationID");
  RequiredParams_.push_back("PixelResolution_nm");
  RequiredParams_.push_back("ImageWidth_px");
  RequiredParams_.push_back("ImageHeight_px");
  RequiredParams_.push_back("SliceThickness_nm");
  RequiredParams_.push_back("ScanRegionOverlap_percent");
  

  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES/VSDA/EM/SetupMicroscope");
  RouteResource->set_method_handler("GET", Callback);
  _Service.publish(RouteResource);


}

Route::~Route() {

}


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
    float PixelResolution_nm = Request->get_query_parameter("PixelResolution_nm", -1);
    int ImageWidth_px = Request->get_query_parameter("ImageWidth_px", -1);
    int ImageHeight_px = Request->get_query_parameter("ImageHeight_px", -1);
    float SliceThickness_nm = Request->get_query_parameter("SliceThickness_nm", -1);
    float ScanRegionOverlap_percent = Request->get_query_parameter("ScanRegionOverlap_percent", -1);


    // Upstream Query
    nlohmann::json UpstreamQuery;
    UpstreamQuery["SimulationID"] = SimID;
    UpstreamQuery["PixelResolution_nm"] = PixelResolution_nm;
    UpstreamQuery["ImageWidth_px"] = ImageWidth_px;
    UpstreamQuery["ImageHeight_px"] = ImageHeight_px;
    UpstreamQuery["SliceThickness_nm"] = SliceThickness_nm;
    UpstreamQuery["ScanRegionOverlap_percent"] = ScanRegionOverlap_percent;


    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, "VSDA/EM/SetupMicroscope", UpstreamQuery.dump(), &UpstreamResponseStr);
    if (!UpstreamStatus) {
      Util::SendCode(_Session.get(), 3);
      return;
    }
    nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);



    // Build Response And Send
    nlohmann::json Response;
    Response["StatusCode"] = 0;
    
    std::cout<<"VSDA EM SetupMicroscope Called With Sim ID: "<<SimID<<std::endl;

    Util::SendJSON(_Session.get(), &Response);
}

}; // Close Namespace
}; // Close Namespace
}; // Close Namespace
}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG