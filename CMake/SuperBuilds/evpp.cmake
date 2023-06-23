# BG Superbuild


# Configuration Parameters
set(TARGET_NAME evpp)
set(LIB_SOURCE_DIR ${LIB_DIR}/SuperBuild/evpp-0.7.0)


# First part of two, build the library if we're currently doing so
if (USE_SUPERBUILD)

    # Add To Dependencies
    list (APPEND DEPENDENCIES ThirdParty_${TARGET_NAME})

    # Create External Project
    message(STATUS "Configuring Library ${TARGET_NAME}")
    ExternalProject_Add (ThirdParty_${TARGET_NAME}

        SOURCE_DIR ${LIB_SOURCE_DIR}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Install/ThirdParty_${TARGET_NAME}/
                  #  -Devpp_BUILD_TESTS:BOOL=OFF

    )
    message(STATUS "Finished Configuring Library ${TARGET_NAME}")

    message(STATUS "Adding CMake Path Argument")
    list (APPEND EXTRA_CMAKE_ARGS
      -Devpp_INSTALL_DIR=${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Install/ThirdParty_${TARGET_NAME}/  
    )
    message(STATUS "Added CMake Path Argument")

    
# Run second part of build, create target/find library we built in the above superbuild target
else()

    option(evpp_INSTALL_DIR "Location Of evpp Build (Used In Superbuild Process)")
    message(STATUS "${TARGET_NAME} Install Directory: ${evpp_INSTALL_DIR}")


    # Set configuration for evpp package to be found using find_package
    set(evpp_DIR ${evpp_INSTALL_DIR}/lib/cmake/evpp/)

    # Now do the find_package stuff like normal (from the evpp-swagger github example)
    find_package(evpp REQUIRED)
    if(evpp_FOUND)
      message(STATUS "Found evpp version: ${evpp_VERSION_STRING}")
    else()
      message(FATAL_ERROR "Could not find evpp")
    endif()

    include_directories(${evpp_INCLUDE_DIRS})

    # Great, now this can be linked to via evpp, and all headers/libs are taken care of by the above code


endif()