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

    NES_Geometry_Shape_Sphere_Create_ = std::make_shared<Resource::NES::Geometry::Shape::Sphere::Create::Route>(&_Server, _Service);
    NES_Geometry_Shape_Sphere_BulkCreate_ = std::make_shared<Resource::NES::Geometry::Shape::Sphere::BulkCreate::Route>(&_Server, _Service);
    NES_Geometry_Shape_Cylinder_Create_ = std::make_shared<Resource::NES::Geometry::Shape::Cylinder::Create::Route>(&_Server, _Service);
    NES_Geometry_Shape_Cylinder_BulkCreate_ = std::make_shared<Resource::NES::Geometry::Shape::Cylinder::BulkCreate::Route>(&_Server, _Service);
    NES_Geometry_Shape_Box_Create_ = std::make_shared<Resource::NES::Geometry::Shape::Box::Create::Route>(&_Server, _Service);
    NES_Geometry_Shape_Box_BulkCreate_ = std::make_shared<Resource::NES::Geometry::Shape::Box::BulkCreate::Route>(&_Server, _Service);

    NES_Compartment_BS_Create_ = std::make_shared<Resource::NES::Compartment::BS::Create::Route>(&_Server, _Service);
    NES_Compartment_BS_BulkCreate_ = std::make_shared<Resource::NES::Compartment::BS::BulkCreate::Route>(&_Server, _Service);


    NES_Connection_Staple_Create_ = std::make_shared<Resource::NES::Connection::Staple::Create::Route>(&_Server, _Service);
    NES_Connection_Receptor_Create_ = std::make_shared<Resource::NES::Connection::Receptor::Create::Route>(Logger_, &_Server, _Service);

    NES_Neuron_BSNeuron_Create = std::make_shared<Resource::NES::Neuron::BSNeuron::Create::Route>(Logger_, &_Server, _Service);

    NES_Tool_PatchClampDAC_Create_ = std::make_shared<Resource::NES::Tool::PatchClampDAC::Create::Route>(Logger_, &_Server, _Service);
    NES_Tool_PatchClampDAC_SetOutputList_ = std::make_shared<Resource::NES::Tool::PatchClampDAC::SetOutputList::Route>(Logger_, &_Server, _Service);

    NES_Tool_PatchClampADC_Create_ = std::make_shared<Resource::NES::Tool::PatchClampADC::Create::Route>(Logger_, &_Server, _Service);
    NES_Tool_PatchClampADC_SetSampleRate_ = std::make_shared<Resource::NES::Tool::PatchClampADC::SetSampleRate::Route>(Logger_, &_Server, _Service);
    NES_Tool_PatchClampADC_GetRecordedData_ = std::make_shared<Resource::NES::Tool::PatchClampADC::GetRecordedData::Route>(Logger_, &_Server, _Service);
    
    NES_Simulation_Create_       = std::make_shared<Resource::NES::Simulation::Create::Route>(Logger_, &_Server, _Service);
    NES_Simulation_Reset_        = std::make_shared<Resource::NES::Simulation::Reset::Route>(&_Server, _Service);
    NES_Simulation_GetStatus_    = std::make_shared<Resource::NES::Simulation::GetStatus::Route>(&_Server, _Service);
    NES_Simulation_RunFor_       = std::make_shared<Resource::NES::Simulation::RunFor::Route>(&_Server, _Service);
    NES_Simulation_RecordAll_    = std::make_shared<Resource::NES::Simulation::RecordAll::Route>(&_Server, _Service);
    NES_Simulation_GetRecording_ = std::make_shared<Resource::NES::Simulation::GetRecording::Route>(Logger_, &_Server, _Service);
    NES_Simulation_BuildMesh_    = std::make_shared<Resource::NES::Simulation::BuildMesh::Route>(Logger_, &_Server, _Service);

    NES_VSDA_EM_Initialize_ = std::make_shared<Resource::NES::VSDA::EM::Initialize::Route>(&_Server, _Service);
    NES_VSDA_EM_SetupMicroscope_ = std::make_shared<Resource::NES::VSDA::EM::SetupMicroscope::Route>(&_Server, _Service);

    NES_VSDA_EM_DefineScanRegion_ = std::make_shared<Resource::NES::VSDA::EM::DefineScanRegion::Route>(&_Server, _Service);
    NES_VSDA_EM_QueueRenderOperation_ = std::make_shared<Resource::NES::VSDA::EM::QueueRenderOperation::Route>(&_Server, _Service);
    NES_VSDA_EM_GetImageStack_ = std::make_shared<Resource::NES::VSDA::EM::GetImageStack::Route>(&_Server, _Service);
    NES_VSDA_EM_GetImage_ = std::make_shared<Resource::NES::VSDA::EM::GetImage::Route>(&_Server, _Service);
    NES_VSDA_EM_GetRenderStatus_ = std::make_shared<Resource::NES::VSDA::EM::GetRenderStatus::Route>(&_Server, _Service);

    NES_VSDA_Calcium_DefineScanRegion_ = std::make_shared<Resource::NES::VSDA::Calcium::DefineScanRegion::Route>(&_Server, _Service);
    NES_VSDA_Calcium_QueueRenderOperation_ = std::make_shared<Resource::NES::VSDA::Calcium::QueueRenderOperation::Route>(&_Server, _Service);
    NES_VSDA_Calcium_GetImageStack_ = std::make_shared<Resource::NES::VSDA::Calcium::GetImageStack::Route>(&_Server, _Service);
    NES_VSDA_Calcium_GetImage_ = std::make_shared<Resource::NES::VSDA::Calcium::GetImage::Route>(&_Server, _Service);
    NES_VSDA_Calcium_GetRenderStatus_ = std::make_shared<Resource::NES::VSDA::Calcium::GetRenderStatus::Route>(&_Server, _Service);
    NES_VSDA_Calcium_CreateIndicator_ = std::make_shared<Resource::NES::VSDA::Calcium::CreateIndicator::Route>(Logger_, &_Server, _Service);
    NES_VSDA_Calcium_Setup_ = std::make_shared<Resource::NES::VSDA::Calcium::Setup::Route>(&_Server, _Service);
}

}; // namespace Server
}; // namespace API
}; // namespace BG
