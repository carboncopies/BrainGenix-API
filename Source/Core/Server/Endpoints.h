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
  #include <memory>
  #include <string>

  // Third-Party Libraries (BG convention: use <> instead of "")
  #include <restbed>

  // Internal Libraries (BG convention: use <> instead of "")
  #include <Server/Server.h>

  #include <Resource/Forbidden.h>
  #include <Resource/NotFound.h>

  #include <Resource/Hello.h>
  #include <Resource/Root.h>
  #include <Resource/Status.h>
  #include <Resource/Version.h>

  #include <Resource/Auth/GetToken.h>

  #include <Resource/NES/Geometry/Shape/BoxCreate.h>
  #include <Resource/NES/Geometry/Shape/BulkBoxCreate.h>
  #include <Resource/NES/Geometry/Shape/CylinderCreate.h>
  #include <Resource/NES/Geometry/Shape/BulkCylinderCreate.h>
  #include <Resource/NES/Geometry/Shape/SphereCreate.h>
  #include <Resource/NES/Geometry/Shape/BulkSphereCreate.h>

  #include <Resource/NES/BSCreate.h>
  #include <Resource/NES/BulkBSCreate.h>

  #include <Resource/NES/ReceptorCreate.h>
  #include <Resource/NES/StapleCreate.h>

  #include <Resource/NES/NeuronCreate.h>

  #include <Resource/NES/PatchClampADCCreate.h>
  #include <Resource/NES/PatchClampADCGetRecordedData.h>
  #include <Resource/NES/PatchClampADCSetSampleRate.h>
  #include <Resource/NES/PatchClampDACCreate.h>
  #include <Resource/NES/PatchClampDACSetOutputList.h>

  #include <Resource/NES/SimulationBuildMesh.h>
  #include <Resource/NES/SimulationCreate.h>
  #include <Resource/NES/SimulationGetRecording.h>
  #include <Resource/NES/SimulationGetStatus.h>
  #include <Resource/NES/SimulationRecordAll.h>
  #include <Resource/NES/SimulationReset.h>
  #include <Resource/NES/SimulationRunFor.h>

  #include <Resource/NES/VSDA/EM/DefineScanRegion.h>
  #include <Resource/NES/VSDA/EM/GetImage.h>
  #include <Resource/NES/VSDA/EM/GetImageStack.h>
  #include <Resource/NES/VSDA/EM/GetRenderStatus.h>
  #include <Resource/NES/VSDA/EM/Initialize.h>
  #include <Resource/NES/VSDA/EM/QueueRenderOperation.h>
  #include <Resource/NES/VSDA/EM/SetupMicroscope.h>

  #include <Resource/NES/VSDA/Calcium/CreateIndicator.h>
  #include <Resource/NES/VSDA/Calcium/DefineScanRegion.h>
  #include <Resource/NES/VSDA/Calcium/GetImage.h>
  #include <Resource/NES/VSDA/Calcium/GetImageStack.h>
  #include <Resource/NES/VSDA/Calcium/GetRenderStatus.h>
  #include <Resource/NES/VSDA/Calcium/QueueRenderOperation.h>
  #include <Resource/NES/VSDA/Calcium/Setup.h>

  #include <Resource/NES/NES.h>

  #include <Resource/NES/Visualizer/RenderImage.h>


  #include <BG/Common/Logger/Logger.h>


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
      std::shared_ptr<Resource::NES::Geometry::Shape::Sphere::BulkCreate::Route> NES_Geometry_Shape_Sphere_BulkCreate_;
      std::shared_ptr<Resource::NES::Geometry::Shape::Cylinder::Create::Route> NES_Geometry_Shape_Cylinder_Create_;
      std::shared_ptr<Resource::NES::Geometry::Shape::Cylinder::BulkCreate::Route> NES_Geometry_Shape_Cylinder_BulkCreate_;
      std::shared_ptr<Resource::NES::Geometry::Shape::Box::Create::Route> NES_Geometry_Shape_Box_Create_;
      std::shared_ptr<Resource::NES::Geometry::Shape::Box::BulkCreate::Route> NES_Geometry_Shape_Box_BulkCreate_;

      std::shared_ptr<Resource::NES::Compartment::BS::Create::Route> NES_Compartment_BS_Create_;
      std::shared_ptr<Resource::NES::Compartment::BS::BulkCreate::Route> NES_Compartment_BS_BulkCreate_;

      std::shared_ptr<Resource::NES::Connection::Staple::Create::Route> NES_Connection_Staple_Create_;
      std::shared_ptr<Resource::NES::Connection::Receptor::Create::Route> NES_Connection_Receptor_Create_;

      std::shared_ptr<Resource::NES::Neuron::BSNeuron::Create::Route> NES_Neuron_BSNeuron_Create;

      std::shared_ptr<Resource::NES::Tool::PatchClampDAC::Create::Route> NES_Tool_PatchClampDAC_Create_;
      std::shared_ptr<Resource::NES::Tool::PatchClampDAC::SetOutputList::Route> NES_Tool_PatchClampDAC_SetOutputList_;

      std::shared_ptr<Resource::NES::Tool::PatchClampADC::Create::Route> NES_Tool_PatchClampADC_Create_;
      std::shared_ptr<Resource::NES::Tool::PatchClampADC::SetSampleRate::Route> NES_Tool_PatchClampADC_SetSampleRate_;
      std::shared_ptr<Resource::NES::Tool::PatchClampADC::GetRecordedData::Route> NES_Tool_PatchClampADC_GetRecordedData_;

      std::shared_ptr<Resource::NES::Simulation::Create::Route> NES_Simulation_Create_;
      std::shared_ptr<Resource::NES::Simulation::Reset::Route> NES_Simulation_Reset_;
      std::shared_ptr<Resource::NES::Simulation::GetStatus::Route> NES_Simulation_GetStatus_;
      std::shared_ptr<Resource::NES::Simulation::RunFor::Route> NES_Simulation_RunFor_;
      std::shared_ptr<Resource::NES::Simulation::RecordAll::Route> NES_Simulation_RecordAll_;
      std::shared_ptr<Resource::NES::Simulation::GetRecording::Route> NES_Simulation_GetRecording_;
      std::shared_ptr<Resource::NES::Simulation::BuildMesh::Route> NES_Simulation_BuildMesh_;

      std::shared_ptr<Resource::NES::VSDA::EM::Initialize::Route> NES_VSDA_EM_Initialize_;
      std::shared_ptr<Resource::NES::VSDA::EM::SetupMicroscope::Route> NES_VSDA_EM_SetupMicroscope_;
      std::shared_ptr<Resource::NES::VSDA::EM::DefineScanRegion::Route> NES_VSDA_EM_DefineScanRegion_;
      std::shared_ptr<Resource::NES::VSDA::EM::QueueRenderOperation::Route> NES_VSDA_EM_QueueRenderOperation_;
      std::shared_ptr<Resource::NES::VSDA::EM::GetImageStack::Route> NES_VSDA_EM_GetImageStack_;
      std::shared_ptr<Resource::NES::VSDA::EM::GetImage::Route> NES_VSDA_EM_GetImage_;
      std::shared_ptr<Resource::NES::VSDA::EM::GetRenderStatus::Route> NES_VSDA_EM_GetRenderStatus_;

      std::shared_ptr<Resource::NES::VSDA::Calcium::CreateIndicator::Route> NES_VSDA_Calcium_CreateIndicator_;
      std::shared_ptr<Resource::NES::VSDA::Calcium::Setup::Route> NES_VSDA_Calcium_Setup_;
      std::shared_ptr<Resource::NES::VSDA::Calcium::DefineScanRegion::Route> NES_VSDA_Calcium_DefineScanRegion_;
      std::shared_ptr<Resource::NES::VSDA::Calcium::GetRenderStatus::Route> NES_VSDA_Calcium_GetRenderStatus_;
      std::shared_ptr<Resource::NES::VSDA::Calcium::QueueRenderOperation::Route> NES_VSDA_Calcium_QueueRenderOperation_;
      std::shared_ptr<Resource::NES::VSDA::Calcium::GetImageStack::Route> NES_VSDA_Calcium_GetImageStack_;
      std::shared_ptr<Resource::NES::VSDA::Calcium::GetImage::Route> NES_VSDA_Calcium_GetImage_;


      std::shared_ptr<Resource::NES::Visualizer::RenderImage::Route> NES_Visualizer_RenderImage_;

      std::shared_ptr<Resource::NES::Route> NES_;



      BG::Common::Logger::LoggingSystem* Logger_;

    public:
      /**
       * @brief Construct a new Endpoint Manager object
       *
       */
      EndpointManager(BG::Common::Logger::LoggingSystem* _Logger);

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

  }; // namespace Server
  }; // namespace API
  }; // namespace BG
