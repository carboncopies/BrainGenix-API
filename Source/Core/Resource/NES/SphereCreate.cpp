#include <Resource/NES/SphereCreate.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace Geometry {
namespace Shape {
namespace Sphere {
namespace Create {

Route::Route(Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;

  // Setup List Of Params
  RequiredParams_.push_back("Radius_nm");
  RequiredParams_.push_back("Center_nm");
  
  OptionalParams_.push_back("Name");


  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES/Geometry/Shape/Sphere/Create");
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
      Util::SendInvalidParamResponse(_Session.get());
      return;
    }

    // Set Defaults
    float InvalidRadius_nm = -1;
    std::string InvalidCenter_nm = "undefined";


    // Get Params
    float Radius_nm = Request->get_query_parameter("Radius_nm", InvalidRadius_nm);
    std::string Center_nm = Request->get_query_parameter("Center_nm", InvalidCenter_nm);
    std::string Name = Request->get_query_parameter("Name", "undefined");

    // Validate Input
    bool IsInputValid = true;
    IsInputValid &= (Radius_nm != InvalidRadius_nm);
    IsInputValid &= (Center_nm != InvalidCenter_nm);



    // Build Response
    nlohmann::json Response;
    if (IsInputValid) {
      Response["StatusCode"] = 3;
      Response["ShapeID"] = -1;
    } else {
      Response["StatusCode"] = 2;
      Response["ShapeID"] = -1;
    }

    // Return Response String As JSON
    std::string Body = Response.dump();
    _Session->close(restbed::OK, Body,
      {
        {"Content-Length", std::to_string(Body.size())},
        {"Content-Type", "application/json"}
      }
    );
}

}; // Close Namespace
}; // Close Namespace
}; // Close Namespace
}; // Close Namespace
}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG