


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

  ${SRC_DIR}/Core/Resource/Dataset/Dataset.cpp
  ${SRC_DIR}/Core/Resource/Dataset/Dataset.h




  ${SRC_DIR}/Core/Resource/NES/NES.h
  ${SRC_DIR}/Core/Resource/NES/NES.cpp
  ${SRC_DIR}/Core/Resource/EVM/EVM.h
  ${SRC_DIR}/Core/Resource/EVM/EVM.cpp


  ${SRC_DIR}/Core/Resource/NES/Visualizer/RenderImage.h
  ${SRC_DIR}/Core/Resource/NES/Visualizer/RenderImage.cpp


  ${SRC_DIR}/Core/NeuroglancerWrapper/NeuroglancerWrapper.h
  ${SRC_DIR}/Core/NeuroglancerWrapper/NeuroglancerWrapper.cpp


  ${SRC_DIR}/Core/Database/DBManager.cpp
  ${SRC_DIR}/Core/Database/DBManager.h

  ${SRC_DIR}/Core/RPC/ClientManager.cpp
  ${SRC_DIR}/Core/RPC/ClientManager.h
  ${SRC_DIR}/Core/RPC/RPCManager.cpp
  ${SRC_DIR}/Core/RPC/RPCManager.h
  ${SRC_DIR}/Core/RPC/StaticRoutes.cpp
  ${SRC_DIR}/Core/RPC/StaticRoutes.h
  ${SRC_DIR}/Core/RPC/ManagerTaskData.cpp
  ${SRC_DIR}/Core/RPC/ManagerTaskData.h
  ${SRC_DIR}/Core/RPC/RouteAndHandler.cpp
  ${SRC_DIR}/Core/RPC/RouteAndHandler.h
  ${SRC_DIR}/Core/RPC/APIStatusCode.cpp
  ${SRC_DIR}/Core/RPC/APIStatusCode.h
  ${SRC_DIR}/Core/RPC/RPCHandlerHelper.cpp
  ${SRC_DIR}/Core/RPC/RPCHandlerHelper.h

  ${SRC_DIR}/Core/Util/Types.cpp
  ${SRC_DIR}/Core/Util/Types.h
  ${SRC_DIR}/Core/Util/ParamCheck.cpp
  ${SRC_DIR}/Core/Util/ParamCheck.h
  ${SRC_DIR}/Core/Util/RestbedHelpers.cpp
  ${SRC_DIR}/Core/Util/RestbedHelpers.h
  ${SRC_DIR}/Core/Util/RPCHelpers.cpp
  ${SRC_DIR}/Core/Util/RPCHelpers.h

  # ${CMAKE_BINARY_DIR}/PythonVersion.h

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

    # pybind11::lto
    # pybind11::embed
    # pybind11::module
    # pybind11::headers

    bg-common-logger

    VersioningSystem
    CompileTimeStamp
    CompilePlatformInfo
    )
target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR}/Core)
target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR}/Shared)
target_include_directories(${PROJECT_NAME} PRIVATE ${CPP_BASE64_INCLUDE_DIRS})