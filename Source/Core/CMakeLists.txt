


# Configure Sources
set(MAIN_SOURCES 


  ${SRC_DIR}/Core/Main.cpp
  ${SRC_DIR}/Core/Main.h

  
  ${SRC_DIR}/Core/Config/ConfigParser.cpp
  ${SRC_DIR}/Core/Config/ConfigParser.h
  ${SRC_DIR}/Core/Config/ConfigManager.cpp
  ${SRC_DIR}/Core/Config/ConfigManager.h

  ${SRC_DIR}/Core/Server/Server.cpp
  ${SRC_DIR}/Core/Server/Server.h

  ${SRC_DIR}/Core/Cluster/VSDA/VSDAConnectionManager.cpp
  ${SRC_DIR}/Core/Cluster/VSDA/VSDAConnectionManager.h

  ${SRC_DIR}/Core/Cluster/BidirectionalRPC.cpp
  ${SRC_DIR}/Core/Cluster/BidirectionalRPC.h

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

  ${SRC_DIR}/Core/Resource/CentralizedRequestHandler/CentralizedRequestHandler.cpp
  ${SRC_DIR}/Core/Resource/CentralizedRequestHandler/CentralizedRequestHandler.h

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


# ========================================================
# Test Configuration - Auto-detect test files
# ========================================================
option(BUILD_TESTING "Build the testing tree" ON)

if(BUILD_TESTING)
    # Find GTest - try REQUIRED first to get the GoogleTest module
    find_package(GTest QUIET)
    
    if(NOT GTest_FOUND)
        # If GTest is not found, use FetchContent
        include(FetchContent)
        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        )
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()

    # Include GoogleTest module for gtest_discover_tests
    include(GoogleTest)

    enable_testing()

    # Automatically glob all test files
    file(GLOB_RECURSE TEST_SOURCES
        "${SRC_DIR}/Core/*.test.cpp"
        "${SRC_DIR}/Core/Tests/*.cpp"
        "${SRC_DIR}/Core/Tests/*.h"
    )

    # Create test executable if test sources exist
    if(TEST_SOURCES)
        message(STATUS "Found test sources: ${TEST_SOURCES}")

        # Get all source files needed for tests (exclude Main.cpp and Main.h)
        set(TEST_NEEDED_SOURCES ${MAIN_SOURCES})
        list(FILTER TEST_NEEDED_SOURCES EXCLUDE REGEX ".*Main\\.(cpp|h)$")

        # Create combined test executable
        add_executable(${PROJECT_NAME}-Test ${TEST_SOURCES} ${TEST_NEEDED_SOURCES})
        target_compile_features(${PROJECT_NAME}-Test PRIVATE cxx_std_17)

        # Include directories
        target_include_directories(${PROJECT_NAME}-Test PRIVATE
            ${SRC_DIR}/Core
            ${SRC_DIR}/Shared
            ${SRC_DIR}/Interface/Configuration/VersioningSystem
        )

        # Link test executable with GTest and all required libraries
        if(TARGET GTest::gtest_main)
            target_link_libraries(${PROJECT_NAME}-Test PRIVATE
                GTest::gtest_main
                GTest::gtest
                GTest::gmock
                GTest::gmock_main
                nlohmann_json::nlohmann_json
                unofficial::restbed::restbed
                oatpp::oatpp
                oatpp::oatpp-openssl
                Boost::program_options
                rpclib::rpc
                spdlog::spdlog
                unofficial::libmariadb
                bg-common-logger
                VersioningSystem
                CompileTimeStamp
                CompilePlatformInfo
                yaml-cpp::yaml-cpp
            )
        else()
            target_link_libraries(${PROJECT_NAME}-Test PRIVATE
                gtest_main
                gtest
                gmock
                gmock_main
                nlohmann_json::nlohmann_json
                unofficial::restbed::restbed
                oatpp::oatpp
                oatpp::oatpp-openssl
                Boost::program_options
                rpclib::rpc
                spdlog::spdlog
                unofficial::libmariadb
                bg-common-logger
                VersioningSystem
                CompileTimeStamp
                CompilePlatformInfo
                yaml-cpp::yaml-cpp
            )
        endif()

        # Discover and register tests automatically
        gtest_discover_tests(${PROJECT_NAME}-Test
            EXTRA_ARGS "--gtest_output=xml:${BIN_DIR}/test_results/"
        )

        # Set test output directory to main binaries directory
        set_target_properties(${PROJECT_NAME}-Test PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
            RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}
            RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}
        )

        message(STATUS "Test executable ${PROJECT_NAME}-Test will be built with all discovered tests")
    else()
        message(STATUS "No test sources found")
    endif()
endif()