


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

  ${SRC_DIR}/Core/Resource/NES/BoxCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/BoxCreate.h
  ${SRC_DIR}/Core/Resource/NES/CylinderCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/CylinderCreate.h
  ${SRC_DIR}/Core/Resource/NES/SphereCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/SphereCreate.h

  ${SRC_DIR}/Core/Resource/NES/BSCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/BSCreate.h

  ${SRC_DIR}/Core/Resource/NES/StapleCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/StapleCreate.h
  ${SRC_DIR}/Core/Resource/NES/ReceptorCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/ReceptorCreate.h

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

  ${SRC_DIR}/Core/Resource/NES/VSDA/EMInitialize.cpp
  ${SRC_DIR}/Core/Resource/NES/VSDA/EMInitialize.h
  ${SRC_DIR}/Core/Resource/NES/VSDA/EMSetupMicroscope.cpp
  ${SRC_DIR}/Core/Resource/NES/VSDA/EMSetupMicroscope.h
  ${SRC_DIR}/Core/Resource/NES/VSDA/EMGetImageStack.h
  ${SRC_DIR}/Core/Resource/NES/VSDA/EMGetImageStack.cpp


  ${SRC_DIR}/Core/Resource/NES/Debug/Debug.h
  ${SRC_DIR}/Core/Resource/NES/Debug/Debug.cpp

  
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


# Include Shared Libs
include(${SRC_DIR}/Shared/Logger/Logger.cmake)


# Create Main Executable
include(${CMAKE_SCRIPTS_DIR}/LinuxBundle/LinuxBundle.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    nlohmann_json::nlohmann_json
    unofficial::restbed::restbed
    Boost::program_options
    rpclib::rpc
    spdlog::spdlog
    unofficial::libmariadb

    VersioningSystem
    CompileTimeStamp
    CompilePlatformInfo
    )
target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR}/Core)
target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR}/Shared)