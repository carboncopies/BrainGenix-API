#include <Resource/NES/NeuronCreate.h>


namespace BG {
namespace API {
namespace Resource {

namespace NES {
namespace Neuron {
namespace BSNeuron {
namespace Create {

Route::Route(BG::Common::Logger::LoggingSystem* _Logger,Server::Server *_Server, restbed::Service &_Service) {
  Server_ = _Server;
  Logger_ = _Logger;

  // Setup List Of Params
  RequiredParams_.push_back("SimulationID");
  RequiredParams_.push_back("SomaID");
  RequiredParams_.push_back("AxonID");
  RequiredParams_.push_back("MembranePotential_mV");
  RequiredParams_.push_back("RestingPotential_mV");
  RequiredParams_.push_back("SpikeThreshold_mV");
  RequiredParams_.push_back("DecayTime_ms");
  RequiredParams_.push_back("AfterHyperpolarizationAmplitude_mV");
  RequiredParams_.push_back("PostsynapticPotentialRiseTime_ms");
  RequiredParams_.push_back("PostsynapticPotentialDecayTime_ms");
  RequiredParams_.push_back("PostsynapticPotentialAmplitude_mV");


  OptionalParams_.push_back("Name");


  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();
  RouteResource->set_path("/NES/Neuron/BSNeuron/Create");
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

    

    // Get Params, Build Upstream Query
    nlohmann::json UpstreamQuery;
    UpstreamQuery["SimulationID"] = Request->get_query_parameter("SimulationID", -1);
    UpstreamQuery["Name"] = Request->get_query_parameter("Name", "undefined");
    
    UpstreamQuery["SomaID"] = Request->get_query_parameter("SomaID", -1);
    UpstreamQuery["AxonID"] = Request->get_query_parameter("AxonID", -1);
    UpstreamQuery["MembranePotential_mV"] = Request->get_query_parameter("MembranePotential_mV", 0.0f);
    UpstreamQuery["RestingPotential_mV"] = Request->get_query_parameter("RestingPotential_mV", 0.0f);
    UpstreamQuery["SpikeThreshold_mV"] = Request->get_query_parameter("SpikeThreshold_mV", 0.0f);
    UpstreamQuery["DecayTime_ms"] = Request->get_query_parameter("DecayTime_ms", 0.0f);
    UpstreamQuery["AfterHyperpolarizationAmplitude_mV"] = Request->get_query_parameter("AfterHyperpolarizationAmplitude_mV", 0.0f);
    UpstreamQuery["PostsynapticPotentialRiseTime_ms"] = Request->get_query_parameter("PostsynapticPotentialRiseTime_ms", 0.0f);
    UpstreamQuery["PostsynapticPotentialDecayTime_ms"] = Request->get_query_parameter("PostsynapticPotentialDecayTime_ms", 0.0f);
    UpstreamQuery["PostsynapticPotentialAmplitude_mV"] = Request->get_query_parameter("PostsynapticPotentialAmplitude_mV", 0.0f);


    std::string UpstreamResponseStr = "";
    bool UpstreamStatus = Util::NESQueryJSON(Server_->NESClient, "Neuron/BSNeuron/Create", UpstreamQuery.dump(), &UpstreamResponseStr);
    if (!UpstreamStatus) {
      Util::SendCode(_Session.get(), 3);
      return;
    }
    //nlohmann::json UpstreamResponse = nlohmann::json::parse(UpstreamResponseStr);



    // Build Response And Send
    // nlohmann::json Response;
    // Response["StatusCode"] = 0;
    // Response["NeuronID"] = UpstreamResponse["NeuronID"].template get<int>();

    //Logger_->Log("Creating Neuron with ID "+ std::to_string(static_cast<int>(Response["NeuronID"]))+'\n',1);

    //Util::SendJSON(_Session.get(), &Response);
    Logger_->Log("Creating Neuron: "+UpstreamResponseStr+'\n',1);
    Util::SendStringifiedJSON(_Session.get(), UpstreamResponseStr);
}

}; // Close Namespace
}; // Close Namespace
}; // Close Namespace
}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG
