# BG Superbuild


# Configuration Parameters
set(TARGET_NAME cpp-httplib)
set(LIB_SOURCE_DIR ${LIB_DIR}/SuperBuild/cpp-httplib-0.12.6)


# First part of two, build the library if we're currently doing so
if (USE_SUPERBUILD)

    # Add To Dependencies
    list (APPEND DEPENDENCIES ThirdParty_${TARGET_NAME})

    # Create External Project
    message(STATUS "Configuring Library ${TARGET_NAME}")
    ExternalProject_Add (ThirdParty_${TARGET_NAME}

        SOURCE_DIR ${LIB_SOURCE_DIR}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Install/ThirdParty_${TARGET_NAME}/
                  #  -Dcpp-httplib_BUILD_TESTS:BOOL=OFF

    )
    message(STATUS "Finished Configuring Library ${TARGET_NAME}")

    message(STATUS "Adding CMake Path Argument")
    list (APPEND EXTRA_CMAKE_ARGS
      -Dhttplib_INSTALL_DIR=${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Install/ThirdParty_${TARGET_NAME}/  
    )
    message(STATUS "Added CMake Path Argument")

    
# Run second part of build, create target/find library we built in the above superbuild target
else()

    option(httplib_INSTALL_DIR "Location Of cpp-httplib Build (Used In Superbuild Process)")
    message(STATUS "${TARGET_NAME} Install Directory: ${cpp-httplib_INSTALL_DIR}")


    # Set configuration for cpp-httplib package to be found using find_package
    set(httplib_DIR ${httplib_INSTALL_DIR}/lib/cmake/httplib/)

    # Now do the find_package stuff like normal (from the cpp-httplib-swagger github example)
    find_package(httplib REQUIRED)
    if(httplib_FOUND)
      message(STATUS "Found cpp-httplib version: ${httplib_VERSION_STRING}")
    else()
      message(FATAL_ERROR "Could not find httplib")
    endif()

    include_directories(${httplib_INCLUDE_DIRS})

    # Great, now this can be linked to via httplib, and all headers/libs are taken care of by the above code


endif()