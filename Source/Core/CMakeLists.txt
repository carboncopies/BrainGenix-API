


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
  

  ${SRC_DIR}/Core/Server/Server.cpp
  ${SRC_DIR}/Core/Server/Server.h

  ${SRC_DIR}/Core/Resource/VSDA/VSDAManager.cpp
  ${SRC_DIR}/Core/Resource/VSDA/VSDAManager.h

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
    oatpp::oatpp
    oatpp::oatpp-openssl
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
target_include_directories(${PROJECT_NAME} PRIVATE ${LIB_DIR}/jwt-cpp/include)

