


# Configure Sources
set(MAIN_SOURCES 
  ${SRC_DIR}/Core/Main.cpp
  ${SRC_DIR}/Core/Main.h

  ${SRC_DIR}/Core/Config/ConfigurationManager.cpp
  ${SRC_DIR}/Core/Config/ConfigurationManager.h
  ${SRC_DIR}/Core/Config/ArgumentHandler.cpp
  ${SRC_DIR}/Core/Config/ArgumentHandler.h
  ${SRC_DIR}/Core/Config/Config.cpp
  ${SRC_DIR}/Core/Config/Config.h

  ${BACKWARD_ENABLE}
  )


# Create Main Executable
include(${CMAKE_SCRIPTS_DIR}/LinuxBundle/LinuxBundle.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    yaml-cpp
    unofficial::restbed::restbed
    Boost::program_options

    VersioningSystem
    CompileTimeStamp
    CompilePlatformInfo
    )
target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR}/Core)