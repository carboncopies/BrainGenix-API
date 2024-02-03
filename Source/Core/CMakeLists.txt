


# Configure Sources
set(MAIN_SOURCES 


  ${SRC_DIR}/Core/Main.cpp
  ${SRC_DIR}/Core/Main.h

  
  ${SRC_DIR}/Core/Config/ConfigurationManager.cpp
  ${SRC_DIR}/Core/Config/ConfigurationManager.h
  ${SRC_DIR}/Core/Config/ArgumentParser.cpp
  ${SRC_DIR}/Core/Config/ArgumentParser.h
  ${SRC_DIR}/Core/Config/ConfigFileParser.cpp
  ${SRC_DIR}/Core/Config/ConfigFileParser.h
  ${SRC_DIR}/Core/Config/Config.cpp
  ${SRC_DIR}/Core/Config/Config.h
  ${SRC_DIR}/Core/Config/ConfigDefaults.cpp
  ${SRC_DIR}/Core/Config/ConfigDefaults.h
  

  ${SRC_DIR}/Core/Server/Controller.cpp
  ${SRC_DIR}/Core/Server/Controller.h
  ${SRC_DIR}/Core/Server/Server.cpp
  ${SRC_DIR}/Core/Server/Server.h
  ${SRC_DIR}/Core/Server/Endpoints.cpp
  ${SRC_DIR}/Core/Server/Endpoints.h

  ${SRC_DIR}/Core/Resource/Root.cpp
  ${SRC_DIR}/Core/Resource/Root.h
  ${SRC_DIR}/Core/Resource/Hello.cpp
  ${SRC_DIR}/Core/Resource/Hello.h
  ${SRC_DIR}/Core/Resource/Version.cpp
  ${SRC_DIR}/Core/Resource/Version.h
  ${SRC_DIR}/Core/Resource/Status.cpp
  ${SRC_DIR}/Core/Resource/Status.h
  ${SRC_DIR}/Core/Resource/NotFound.cpp
  ${SRC_DIR}/Core/Resource/NotFound.h
  ${SRC_DIR}/Core/Resource/Forbidden.cpp
  ${SRC_DIR}/Core/Resource/Forbidden.h

  ${SRC_DIR}/Core/Resource/Auth/GetToken.cpp
  ${SRC_DIR}/Core/Resource/Auth/GetToken.h

  ${SRC_DIR}/Core/Resource/NES/Geometry/Shape/BoxCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/Geometry/Shape/BoxCreate.h
  ${SRC_DIR}/Core/Resource/NES/Geometry/Shape/BulkBoxCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/Geometry/Shape/BulkBoxCreate.h
  ${SRC_DIR}/Core/Resource/NES/Geometry/Shape/CylinderCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/Geometry/Shape/CylinderCreate.h
  ${SRC_DIR}/Core/Resource/NES/Geometry/Shape/BulkCylinderCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/Geometry/Shape/BulkCylinderCreate.h
  ${SRC_DIR}/Core/Resource/NES/Geometry/Shape/SphereCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/Geometry/Shape/SphereCreate.h
  ${SRC_DIR}/Core/Resource/NES/Geometry/Shape/BulkSphereCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/Geometry/Shape/BulkSphereCreate.h

  ${SRC_DIR}/Core/Resource/NES/BSCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/BSCreate.h
  ${SRC_DIR}/Core/Resource/NES/BulkBSCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/BulkBSCreate.h

  ${SRC_DIR}/Core/Resource/NES/StapleCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/StapleCreate.h
  ${SRC_DIR}/Core/Resource/NES/ReceptorCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/ReceptorCreate.h

  ${SRC_DIR}/Core/Resource/NES/NeuronCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/NeuronCreate.h

  ${SRC_DIR}/Core/Resource/NES/PatchClampDACCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/PatchClampDACCreate.h
  ${SRC_DIR}/Core/Resource/NES/PatchClampDACSetOutputList.cpp
  ${SRC_DIR}/Core/Resource/NES/PatchClampDACSetOutputList.h
  ${SRC_DIR}/Core/Resource/NES/PatchClampADCCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/PatchClampADCCreate.h
  ${SRC_DIR}/Core/Resource/NES/PatchClampADCSetSampleRate.cpp
  ${SRC_DIR}/Core/Resource/NES/PatchClampADCSetSampleRate.h
  ${SRC_DIR}/Core/Resource/NES/PatchClampADCGetRecordedData.cpp
  ${SRC_DIR}/Core/Resource/NES/PatchClampADCGetRecordedData.h

  ${SRC_DIR}/Core/Resource/NES/SimulationCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/SimulationCreate.h
  ${SRC_DIR}/Core/Resource/NES/SimulationReset.cpp
  ${SRC_DIR}/Core/Resource/NES/SimulationReset.h
  ${SRC_DIR}/Core/Resource/NES/SimulationGetStatus.cpp
  ${SRC_DIR}/Core/Resource/NES/SimulationGetStatus.h
  ${SRC_DIR}/Core/Resource/NES/SimulationRunFor.cpp
  ${SRC_DIR}/Core/Resource/NES/SimulationRunFor.h
  ${SRC_DIR}/Core/Resource/NES/SimulationRecordAll.cpp
  ${SRC_DIR}/Core/Resource/NES/SimulationRecordAll.h
  ${SRC_DIR}/Core/Resource/NES/SimulationGetRecording.cpp
  ${SRC_DIR}/Core/Resource/NES/SimulationGetRecording.h
  ${SRC_DIR}/Core/Resource/NES/SimulationBuildMesh.cpp
  ${SRC_DIR}/Core/Resource/NES/SimulationBuildMesh.h

  ${SRC_DIR}/Core/Resource/NES/VSDA/EM/Initialize.cpp
  ${SRC_DIR}/Core/Resource/NES/VSDA/EM/Initialize.h
  ${SRC_DIR}/Core/Resource/NES/VSDA/EM/SetupMicroscope.cpp
  ${SRC_DIR}/Core/Resource/NES/VSDA/EM/SetupMicroscope.h

  ${SRC_DIR}/Core/Resource/NES/VSDA/EM/DefineScanRegion.cpp
  ${SRC_DIR}/Core/Resource/NES/VSDA/EM/DefineScanRegion.h
  ${SRC_DIR}/Core/Resource/NES/VSDA/EM/QueueRenderOperation.cpp
  ${SRC_DIR}/Core/Resource/NES/VSDA/EM/QueueRenderOperation.h
  ${SRC_DIR}/Core/Resource/NES/VSDA/EM/GetImageStack.h
  ${SRC_DIR}/Core/Resource/NES/VSDA/EM/GetImageStack.cpp
  ${SRC_DIR}/Core/Resource/NES/VSDA/EM/GetImage.h
  ${SRC_DIR}/Core/Resource/NES/VSDA/EM/GetImage.cpp
  ${SRC_DIR}/Core/Resource/NES/VSDA/EM/GetRenderStatus.h
  ${SRC_DIR}/Core/Resource/NES/VSDA/EM/GetRenderStatus.cpp
  
  ${SRC_DIR}/Core/Resource/NES/VSDA/Calcium/QueueRenderOperation.h
  ${SRC_DIR}/Core/Resource/NES/VSDA/Calcium/QueueRenderOperation.cpp
  ${SRC_DIR}/Core/Resource/NES/VSDA/Calcium/GetRenderStatus.h
  ${SRC_DIR}/Core/Resource/NES/VSDA/Calcium/GetRenderStatus.cpp
  ${SRC_DIR}/Core/Resource/NES/VSDA/Calcium/DefineScanRegion.h
  ${SRC_DIR}/Core/Resource/NES/VSDA/Calcium/DefineScanRegion.cpp
  ${SRC_DIR}/Core/Resource/NES/VSDA/Calcium/CreateIndicator.cpp
  ${SRC_DIR}/Core/Resource/NES/VSDA/Calcium/CreateIndicator.h
  ${SRC_DIR}/Core/Resource/NES/VSDA/Calcium/GetImageStack.cpp
  ${SRC_DIR}/Core/Resource/NES/VSDA/Calcium/GetImageStack.h
  ${SRC_DIR}/Core/Resource/NES/VSDA/Calcium/GetImage.cpp
  ${SRC_DIR}/Core/Resource/NES/VSDA/Calcium/GetImage.h
  ${SRC_DIR}/Core/Resource/NES/VSDA/Calcium/Setup.h
  ${SRC_DIR}/Core/Resource/NES/VSDA/Calcium/Setup.cpp



  ${SRC_DIR}/Core/Resource/NES/NES.h
  ${SRC_DIR}/Core/Resource/NES/NES.cpp



  ${SRC_DIR}/Core/Resource/NES/Visualizer/RenderImage.h
  ${SRC_DIR}/Core/Resource/NES/Visualizer/RenderImage.cpp




  ${SRC_DIR}/Core/Database/DBManager.cpp
  ${SRC_DIR}/Core/Database/DBManager.h

  ${SRC_DIR}/Core/RPC/ClientManager.cpp
  ${SRC_DIR}/Core/RPC/ClientManager.h

  ${SRC_DIR}/Core/Util/Types.cpp
  ${SRC_DIR}/Core/Util/Types.h
  ${SRC_DIR}/Core/Util/ParamCheck.cpp
  ${SRC_DIR}/Core/Util/ParamCheck.h
  ${SRC_DIR}/Core/Util/RestbedHelpers.cpp
  ${SRC_DIR}/Core/Util/RestbedHelpers.h
  ${SRC_DIR}/Core/Util/RPCHelpers.cpp
  ${SRC_DIR}/Core/Util/RPCHelpers.h

  ${BACKWARD_ENABLE}
  )





# Create Main Executable
include(${CMAKE_SCRIPTS_DIR}/LinuxBundle/LinuxBundle.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    nlohmann_json::nlohmann_json
    unofficial::restbed::restbed
    Boost::program_options
    rpclib::rpc
    spdlog::spdlog
    unofficial::libmariadb

    bg-common-logger

    VersioningSystem
    CompileTimeStamp
    CompilePlatformInfo
    )
target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR}/Core)
target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR}/Shared)
