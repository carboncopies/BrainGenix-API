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
    VERSION "2024.04.10"
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Internal Deps
message(STATUS "Finding bg-common-logger Package")
find_package(bg-common-logger CONFIG REQUIRED)

# Add Libraries
message(STATUS "Setting Up yaml-cpp Library")
find_package(yaml-cpp CONFIG REQUIRED)

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

message(STATUS "Finding Python3 Package")
find_package(Python COMPONENTS Interpreter Development)

message(STATUS "Finding Pybind11 Package")
# find_package(pybind11 CONFIG REQUIRED)
add_subdirectory(${LIB_DIR}/NonSuperBuild/pybind11)

find_path(CPP_BASE64_INCLUDE_DIRS "cpp-base64/base64.cpp")

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

message(STATUS "Adding Configuration File To ${INSTALL_PREFIX}/API.cfg")
install(FILES "${SRC_DIR}/Data/API.cfg"
    DESTINATION ${INSTALL_PREFIX}/Default
    COMPONENT Config
)
# install(CODE "
#     if (NOT EXISTS \"${INSTALL_PREFIX}/API.cfg\")
#         file(INSTALL \"${INSTALL_PREFIX}/API-Default.cfg\" DESTINATION \"${INSTALL_PREFIX}/API.cfg\")
#     endif() "
#     COMPONENT Config
# )

message(STATUS "Installing Unit File To '/etc/systemd/system'")
install(FILES "${SRC_DIR}/Data/BrainGenix-API.service"
    DESTINATION "/etc/systemd/system/"
    COMPONENT Release
)



# CPack Information
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Carboncopies Foundation")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${SRC_DIR}/Data/prerm;${SRC_DIR}/Data/postrm;${SRC_DIR}/Data/postinst;${SRC_DIR}/Data/conffiles")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "python3-pip")
SET(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/Artifacts)
INCLUDE(CPack)