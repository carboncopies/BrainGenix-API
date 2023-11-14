//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file is where all routes are linked into the system
    Additional Notes: None
    Date Created: 2023-06-23
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <string>
#include <memory>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <restbed>

// Internal Libraries (BG convention: use <> instead of "")
#include <Server/Server.h>


#include <Resource/NotFound.h>
#include <Resource/Forbidden.h>

#include <Resource/Root.h>
#include <Resource/Hello.h>
#include <Resource/Version.h>
#include <Resource/Status.h>

#include <Resource/Auth/GetToken.h>

#include <Resource/NES/BoxCreate.h>
#include <Resource/NES/CylinderCreate.h>
#include <Resource/NES/SphereCreate.h>

#include <Resource/NES/BSCreate.h>

#include <Resource/NES/StapleCreate.h>
#include <Resource/NES/ReceptorCreate.h>

#include <Resource/NES/PatchClampDACCreate.h>
#include <Resource/NES/PatchClampDACSetOutputList.h>
#include <Resource/NES/PatchClampADCCreate.h>
#include <Resource/NES/PatchClampADCSetSampleRate.h>
#include <Resource/NES/PatchClampADCGetRecordedData.h>


#include <Resource/NES/SimulationCreate.h>
#include <Resource/NES/SimulationReset.h>
#include <Resource/NES/SimulationRunFor.h>
#include <Resource/NES/SimulationGetStatus.h>
#include <Resource/NES/SimulationRecordAll.h>
#include <Resource/NES/SimulationGetRecording.h>
#include <Resource/NES/SimulationBuildMesh.h>

#include <Resource/NES/VSDA/EMInitialize.h>
#include <Resource/NES/VSDA/EMSetupMicroscope.h>
#include <Resource/NES/VSDA/EMGetImageStack.h>



namespace BG {
namespace API {
namespace Server {

class EndpointManager {

private:

    // Keep a list of resources below, so they stay in scope
    std::shared_ptr<Resource::NotFound::Route> NotFound_;
    std::shared_ptr<Resource::Forbidden::Route> Forbidden_;

    std::shared_ptr<Resource::Root::Route> Root_;
    std::shared_ptr<Resource::Hello::Route> Hello_;
    std::shared_ptr<Resource::Diagnostic::Version::Route> Diagnostic_Version_;
    std::shared_ptr<Resource::Diagnostic::Status::Route> Diagnostic_Status_;

    std::shared_ptr<Resource::Auth::GetToken::Route> Auth_GetToken_;

    std::shared_ptr<Resource::NES::Geometry::Shape::Sphere::Create::Route> NES_Geometry_Shape_Sphere_Create_;
    std::shared_ptr<Resource::NES::Geometry::Shape::Cylinder::Create::Route> NES_Geometry_Shape_Cylinder_Create_;
    std::shared_ptr<Resource::NES::Geometry::Shape::Box::Create::Route> NES_Geometry_Shape_Box_Create_;

    std::shared_ptr<Resource::NES::Compartment::BS::Create::Route> NES_Compartment_BS_Create_;

    std::shared_ptr<Resource::NES::Connection::Staple::Create::Route> NES_Connection_Staple_Create_;
    std::shared_ptr<Resource::NES::Connection::Receptor::Create::Route> NES_Connection_Receptor_Create_;
    
    std::shared_ptr<Resource::NES::Tool::PatchClampDAC::Create::Route> NES_Tool_PatchClampDAC_Create_;
    std::shared_ptr<Resource::NES::Tool::PatchClampDAC::SetOutputList::Route> NES_Tool_PatchClampDAC_SetOutputList_;

    std::shared_ptr<Resource::NES::Tool::PatchClampADC::Create::Route> NES_Tool_PatchClampADC_Create_;
    std::shared_ptr<Resource::NES::Tool::PatchClampADC::SetSampleRate::Route> NES_Tool_PatchClampADC_SetSampleRate_;
    std::shared_ptr<Resource::NES::Tool::PatchClampADC::GetRecordedData::Route> NES_Tool_PatchClampADC_GetRecordedData_;

    std::shared_ptr<Resource::NES::Simulation::Create::Route>       NES_Simulation_Create_;
    std::shared_ptr<Resource::NES::Simulation::Reset::Route>        NES_Simulation_Reset_;
    std::shared_ptr<Resource::NES::Simulation::GetStatus::Route>    NES_Simulation_GetStatus_;
    std::shared_ptr<Resource::NES::Simulation::RunFor::Route>       NES_Simulation_RunFor_;
    std::shared_ptr<Resource::NES::Simulation::RecordAll::Route>    NES_Simulation_RecordAll_;
    std::shared_ptr<Resource::NES::Simulation::GetRecording::Route> NES_Simulation_GetRecording_;
    std::shared_ptr<Resource::NES::Simulation::BuildMesh::Route>    NES_Simulation_BuildMesh_;

    std::shared_ptr<Resource::NES::VSDA::EM::Initialize::Route>     NES_VSDA_EM_Initialize_;
    std::shared_ptr<Resource::NES::VSDA::EM::SetupMicroscope::Route>     NES_VSDA_EM_SetupMicroscope_;
    std::shared_ptr<Resource::NES::VSDA::EM::GetImageStack::Route>     NES_VSDA_EM_GetImageStack_;

public:

    /**
     * @brief Construct a new Endpoint Manager object
     * 
     */
    EndpointManager();

    /**
     * @brief Destroy the Endpoint Manager object
     * 
     */
    ~EndpointManager();


    /**
     * @brief This function is where all routes are added to the service.
     * They're included in this file from the Route directory and then added in the AddRoutes function body.
     * 
     * @param _Server 
     */
    void AddRoutes(restbed::Service &_Service, Server &_Server);


};

}; // Close Namespace Server
}; // Close Namespace API
}; // Close Namespace BG