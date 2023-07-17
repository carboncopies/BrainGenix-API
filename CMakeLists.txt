# Set CMAKE Version, Define Project
cmake_minimum_required (VERSION 3.1...3.21)


# Set Configuration Options
option(ENABLE_BACKWARD "Build BG With Backward to provide some stack trace information or not" ON)
set(INSTALL_PREFIX "/etc/BrainGenix/API")

# Set Non-User Changable Configuration Data (Used during build process )
# option (USE_SUPERBUILD "Whether or not a superbuild should be invoked" ON) # You probably don't want to change this one - used internally


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


# Setup vcpkg script with CMake (note: should be placed before project() call)
set(CMAKE_TOOLCHAIN_FILE ${LIB_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")


project(
    "BrainGenix-API"
    VERSION "0.0.1"
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Add Libraries
message(STATUS "Finding restbed Package")
find_package(unofficial-restbed CONFIG REQUIRED)

message(STATUS "Finding nlohmann_json Package")
find_package(nlohmann_json CONFIG REQUIRED)

message(STATUS "Finding boost Package")
set(Boost_NO_WARN_NEW_VERSIONS 1)
find_package(Boost REQUIRED COMPONENTS program_options)

message(STATUS "Finding rpclib Package")
find_package(rpclib CONFIG REQUIRED)

message(STATUS "Finding spdlog Package")
find_package(spdlog CONFIG REQUIRED)

message(STATUS "Finding libmariadb Package")
find_package(unofficial-libmariadb CONFIG REQUIRED)



# Include Util CMake Scripts
include(${CMAKE_UTIL_DIR}/ColorizeMessages.cmake)
include(${CMAKE_UTIL_DIR}/SetBinaryDir.cmake)
include(${CMAKE_UTIL_DIR}/DisableTargetWarnings.cmake)

# Run Configuration Tools
include(${CMAKE_SCRIPTS_DIR}/VersioningSystem/VersioningSystem.cmake)
include(${CMAKE_SCRIPTS_DIR}/CompilePlatformInfo/CompilePlatformInfo.cmake)
include(${CMAKE_SCRIPTS_DIR}/CompileTimeStamp/CompileTimeStamp.cmake)


add_subdirectory(${SRC_DIR}/Core)


# Set Install Targets
message(STATUS "Install Prefix Set To '${INSTALL_PREFIX}'")
message(STATUS "Executable Will Be Installed To '/usr/bin'")
install(TARGETS ${PROJECT_NAME}
    COMPONENT Release
    RUNTIME DESTINATION "/usr/bin/"
    LIBRARY DESTINATION ${INSTALL_PREFIX}
    DESTINATION ${INSTALL_PREFIX}
)
install(FILES "${SRC_DIR}/Data/API.cfg"
    DESTINATION ${INSTALL_PREFIX}
    COMPONENT Release
)
message(STATUS "Installing Unit File To '/etc/systemd/system'")
install(FILES "${SRC_DIR}/Data/BrainGenix-API.service"
    DESTINATION "/etc/systemd/system/"
    COMPONENT Release
)



# CPack Information
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Carboncopies Foundation")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${SRC_DIR}/Data/prerm;${SRC_DIR}/Data/postrm;${SRC_DIR}/Data/postinst")
SET(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/Artifacts)
INCLUDE(CPack)