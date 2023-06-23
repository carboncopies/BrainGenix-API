# Set CMAKE Version, Define Project
cmake_minimum_required (VERSION 3.1...3.21)


# Set Configuration Options
option(ENABLE_BACKWARD "Build BG With Backward to provide some stack trace information or not" ON)


# Set Non-User Changable Configuration Data (Used during build process )
option (USE_SUPERBUILD "Whether or not a superbuild should be invoked" ON) # You probably don't want to change this one - used internally


# Set Dirs
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")
set(BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Binaries")
set(CMAKE_UTIL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Utils")
set(CMAKE_BUILD_UTILS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMake/BuildUtils")
set(CMAKE_FIND_UTILS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMake/FindUtils")
set(CMAKE_SCRIPTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Scripts")
set(CMAKE_UTILS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
set(CMAKE_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/EditorAssets")
set(PROJECT_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_VISIBILITY_PRESET hidden)








# Superbuild Configuration
if (USE_SUPERBUILD)

  project (SUPERBUILD NONE)

  # Add Superbuild Runner
  include(${CMAKE_UTILS_DIR}/SuperBuildMaster.cmake)
  return()
else()

  project(
      "BrainGenix-API"
      VERSION "0.0.1"
      LANGUAGES CXX
  )

endif()



set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Superbuild Configuration
message(STATUS "Superbuild Configuration:")

include(${CMAKE_UTILS_DIR}/SuperBuilds/YAMLCPP.cmake)
include(${CMAKE_UTILS_DIR}/SuperBuilds/OatPP.cmake)
include(${CMAKE_UTILS_DIR}/SuperBuilds/OatPP-Swagger.cmake)
include(${CMAKE_UTILS_DIR}/SuperBuilds/OatPP-Postgresql.cmake)

# Include Util CMake Scripts
include(${CMAKE_UTIL_DIR}/ColorizeMessages.cmake)
include(${CMAKE_UTIL_DIR}/SetBinaryDir.cmake)
include(${CMAKE_UTIL_DIR}/DisableTargetWarnings.cmake)

# Run Configuration Tools
include(${CMAKE_SCRIPTS_DIR}/VersioningSystem/VersioningSystem.cmake)
include(${CMAKE_SCRIPTS_DIR}/CompilePlatformInfo/CompilePlatformInfo.cmake)
include(${CMAKE_SCRIPTS_DIR}/CompileTimeStamp/CompileTimeStamp.cmake)



# ADD STRUCT INTERFACE SUBDIRECTORIES
BGBuildLogger(${Green} "Configuring Structure Interfaces")

BGBuildLogger(${BoldGreen} "Finished Configuring Structure Interfaces")


# Once we have core subdirs, we can populate this - for now i've commented it out.


# # ADD INTERNAL SUBDIRECTORIES
# BGBuildLogger(${Green} "Configuring Internal Libraries")
# BGBuildLogger(${BoldGreen} "Finished Configuring Internal Libraries")

# # ADD CORE SUBDIRECTORIES
# BGBuildLogger(${Green} "Configuring Core Subsystems")
# add_subdirectory(${SRC_DIR}/Core/Writers/ERS_ProjectWriter)
# BGBuildLogger(${BoldGreen} "Finished Configuring Core Subsystems")


# Configure Sources
set(MAIN_SOURCES 
  ${SRC_DIR}/Core/Main.cpp
  ${SRC_DIR}/Core/Main.h
  ${SRC_DIR}/Core/ApplicationComponent.cpp
  ${SRC_DIR}/Core/ApplicationComponent.h
  ${SRC_DIR}/Core/DTO/DataTransferObjects.cpp
  ${SRC_DIR}/Core/DTO/DataTransferObjects.h
  ${SRC_DIR}/Core/Controller/APIController.cpp
  ${SRC_DIR}/Core/Controller/APIController.h
  ${BACKWARD_ENABLE}
  )


# Create Main Executable
include(${CMAKE_SCRIPTS_DIR}/LinuxBundle/LinuxBundle.cmake)
target_link_libraries(${PROJECT_NAME} yaml-cpp oatpp::oatpp oatpp::oatpp-swagger oatpp::oatpp-postgresql)

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR}/Core)
# target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR}/Core/DTO)
