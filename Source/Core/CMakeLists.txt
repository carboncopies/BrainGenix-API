


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
  ${SRC_DIR}/Core/Resource/NES/SphereCreate.cpp
  ${SRC_DIR}/Core/Resource/NES/SphereCreate.h

  ${SRC_DIR}/Core/Util/Types.cpp
  ${SRC_DIR}/Core/Util/Types.h
  ${SRC_DIR}/Core/Util/ParamCheck.cpp
  ${SRC_DIR}/Core/Util/ParamCheck.h

  ${BACKWARD_ENABLE}
  )


# Create Main Executable
include(${CMAKE_SCRIPTS_DIR}/LinuxBundle/LinuxBundle.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    nlohmann_json::nlohmann_json
    unofficial::restbed::restbed
    Boost::program_options

    VersioningSystem
    CompileTimeStamp
    CompilePlatformInfo
    )
target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR}/Core)