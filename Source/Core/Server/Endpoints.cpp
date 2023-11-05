#include <Server/Endpoints.h>


namespace BG {
namespace API {
namespace Server {


EndpointManager::EndpointManager() {

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

    NES_Geometry_Shape_Sphere_Create_ = std::make_shared<Resource::NES::Geometry::Shape::Sphere::Create::Route>(&_Server, _Service);
    NES_Geometry_Shape_Cylinder_Create_ = std::make_shared<Resource::NES::Geometry::Shape::Cylinder::Create::Route>(&_Server, _Service);
    NES_Geometry_Shape_Box_Create_ = std::make_shared<Resource::NES::Geometry::Shape::Box::Create::Route>(&_Server, _Service);

    NES_Compartment_BS_Create_ = std::make_shared<Resource::NES::Compartment::BS::Create::Route>(&_Server, _Service);

    NES_Connection_Staple_Create_ = std::make_shared<Resource::NES::Connection::Staple::Create::Route>(&_Server, _Service);
    NES_Connection_Receptor_Create_ = std::make_shared<Resource::NES::Connection::Receptor::Create::Route>(&_Server, _Service);

    NES_Tool_PatchClampDAC_Create_ = std::make_shared<Resource::NES::Tool::PatchClampDAC::Create::Route>(&_Server, _Service);
    NES_Tool_PatchClampDAC_SetOutputList_ = std::make_shared<Resource::NES::Tool::PatchClampDAC::SetOutputList::Route>(&_Server, _Service);

    NES_Tool_PatchClampADC_Create_ = std::make_shared<Resource::NES::Tool::PatchClampADC::Create::Route>(&_Server, _Service);
    NES_Tool_PatchClampADC_SetSampleRate_ = std::make_shared<Resource::NES::Tool::PatchClampADC::SetSampleRate::Route>(&_Server, _Service);
    NES_Tool_PatchClampADC_GetRecordedData_ = std::make_shared<Resource::NES::Tool::PatchClampADC::GetRecordedData::Route>(&_Server, _Service);
    
    NES_Simulation_Create_ = std::make_shared<Resource::NES::Simulation::Create::Route>(&_Server, _Service);
    NES_Simulation_Reset_ = std::make_shared<Resource::NES::Simulation::Reset::Route>(&_Server, _Service);
    NES_Simulation_GetStatus_ = std::make_shared<Resource::NES::Simulation::GetStatus::Route>(&_Server, _Service);
    NES_Simulation_RunFor_ = std::make_shared<Resource::NES::Simulation::RunFor::Route>(&_Server, _Service);
    NES_Simulation_RecordAll_ = std::make_shared<Resource::NES::Simulation::RecordAll::Route>(&_Server, _Service);
    NES_Simulation_GetRecording_ = std::make_shared<Resource::NES::Simulation::GetRecording::Route>(&_Server, _Service);
    NES_Simulation_BuildMesh_ = std::make_shared<Resource::NES::Simulation::BuildMesh::Route>(&_Server, _Service);

}


}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG