


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
# Test Configuration
# ========================================================
option(BUILD_TESTING "Build the testing tree" ON)

if(BUILD_TESTING)
    # Find GTest
    find_package(GTest CONFIG QUIET)
    
    if(NOT GTest_FOUND)
        # If GTest is not found via vcpkg, try to use FetchContent
        include(FetchContent)
        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        )
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()

    enable_testing()

    # Test executable for CentralizedRequestHandler
    add_executable(CentralizedRequestHandlerTest
        ${SRC_DIR}/Core/Resource/CentralizedRequestHandler/CentralizedRequestHandler.test.cpp
        ${SRC_DIR}/Core/Resource/CentralizedRequestHandler/CentralizedRequestHandler.cpp
    )

    # Link test executable with GTest and required libraries
    if(TARGET GTest::gtest_main)
        target_link_libraries(CentralizedRequestHandlerTest PRIVATE
            GTest::gtest_main
            GTest::gtest
            VersioningSystem
        )
    else()
        target_link_libraries(CentralizedRequestHandlerTest PRIVATE
            gtest_main
            gtest
            VersioningSystem
        )
    endif()

    # Include directories
    target_include_directories(CentralizedRequestHandlerTest PRIVATE
        ${SRC_DIR}/Core
        ${SRC_DIR}/Shared
        ${SRC_DIR}/Interface/Configuration/VersioningSystem
    )

    # Add test to CTest
    add_test(NAME CentralizedRequestHandlerTest COMMAND CentralizedRequestHandlerTest)
    
    # Also add ConfigParser test if it exists
    if(EXISTS ${SRC_DIR}/Core/Config/ConfigParser.test.cpp)
        add_executable(ConfigParserTest
            ${SRC_DIR}/Core/Config/ConfigParser.test.cpp
            ${SRC_DIR}/Core/Config/ConfigParser.cpp
        )
        
        if(TARGET GTest::gtest_main)
            target_link_libraries(ConfigParserTest PRIVATE
                GTest::gtest_main
                GTest::gtest
            )
        else()
            target_link_libraries(ConfigParserTest PRIVATE
                gtest_main
                gtest
            )
        endif()
        
        target_include_directories(ConfigParserTest PRIVATE
            ${SRC_DIR}/Core
            ${SRC_DIR}/Shared
        )
        
        # Link with required libraries for ConfigParser
        target_link_libraries(ConfigParserTest PRIVATE
            yaml-cpp::yaml-cpp
            bg-common-logger
        )
        
        add_test(NAME ConfigParserTest COMMAND ConfigParserTest)
    endif()
endif()