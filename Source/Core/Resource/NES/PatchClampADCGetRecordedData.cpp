#include <Resource/NES/PatchClampADCGetRecordedData.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace Tool {
namespace PatchClampADC {
namespace GetRecordedData {

Route::Route(Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;

  // Setup List Of Params
  RequiredParams_.push_back("SimulationID");
  RequiredParams_.push_back("TargetADC");


  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES/Tool/PatchClampADC/GetRecordedData");
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
    // float Radius_nm = Request->get_query_parameter("Radius_nm", -1);
    // std::string Center_nm = Request->get_query_parameter("Center_nm", "");
    // std::string Name = Request->get_query_parameter("Name", "undefined");


    // Build Response And Send
    nlohmann::json Response;
    Response["StatusCode"] = 3;
    Response["RecordedData_mV"] = "{}";
    Response["Timestep_ms"] = -1;

    Util::SendJSON(_Session.get(), &Response);
}

}; // Close Namespace
}; // Close Namespace
}; // Close Namespace
}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG