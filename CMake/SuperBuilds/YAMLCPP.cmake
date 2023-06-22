# BG Superbuild


# Configuration Parameters
set(TARGET_NAME YAMLCPP)
set(LIB_SOURCE_DIR ${LIB_DIR}/SuperBuild/yaml-cpp)



# First part of two, build the library if we're currently doing so
if (USE_SUPERBUILD)



    # Add To Dependencies
    list (APPEND DEPENDENCIES ThirdParty_${TARGET_NAME})

    # Create External Project
    message(STATUS "Configuring Library ${TARGET_NAME}")
    ExternalProject_Add (ThirdParty_${TARGET_NAME}

        SOURCE_DIR ${LIB_SOURCE_DIR}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Install/ThirdParty_${TARGET_NAME}/
                   -DYAML_CPP_BUILD_TESTS:BOOL=OFF

    )
    message(STATUS "Finished Configuring Library ${TARGET_NAME}")

    message(STATUS "Adding CMake Path Argument")
    list (APPEND EXTRA_CMAKE_ARGS
      -Dyaml-cpp_INSTALL_DIR=${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Install/ThirdParty_${TARGET_NAME}/  
    )
    message(STATUS "Added CMake Path Argument")

    
# Run second part of build, create target/find library we built in the above superbuild target
else()

    option(yaml-cpp_INSTALL_DIR "Location Of yaml-cpp Build (Used In Superbuild Process)")

    message(STATUS "${TARGET_NAME} Install Directory: ${yaml-cpp_INSTALL_DIR}")


    # Set configuration for oatpp package to be found using find_package
    set(yaml-cpp_DIR ${yaml-cpp_INSTALL_DIR}/share/cmake/yaml-cpp)


    # Now do the find_package stuff like normal (from the oatpp-swagger github example)
    find_package(yaml-cpp REQUIRED)

    if(yaml-cpp_FOUND)
      message(STATUS "Found yaml-cpp version: ${yaml-cpp_VERSION_STRING}")
    else()
      message(FATAL_ERROR "Could not find yaml-cpp")
    endif()

    include_directories(${yaml-cpp_INCLUDE_DIRS})

    # Great, now this can be linked to via oatpp::oatpp, and all headers/libs are taken care of by the above code


endif()