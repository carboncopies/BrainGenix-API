# BG Superbuild


# Configuration Parameters
set(TARGET_NAME OatPP-Swagger)
set(LIB_SOURCE_DIR ${LIB_DIR}/SuperBuild/oatpp-swagger-1.3.0)


# First part of two, build the library if we're currently doing so
if (USE_SUPERBUILD)

    # Add To Dependencies
    list (APPEND DEPENDENCIES ThirdParty_${TARGET_NAME})

    # Create External Project
    message(STATUS "Configuring Library ${TARGET_NAME}")
    ExternalProject_Add (ThirdParty_${TARGET_NAME}

        SOURCE_DIR ${LIB_SOURCE_DIR}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Install/ThirdParty_${TARGET_NAME}/
                   -DOATPP_BUILD_TESTS:BOOL=OFF
                   -Doatpp_DIR=${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Install/ThirdParty_OatPP/lib/cmake/oatpp-1.3.0/  

    )
    add_dependencies(ThirdParty_${TARGET_NAME} ThirdParty_OatPP) # Add dependencies for oatpp since this lib requires it to be created first
    message(STATUS "Finished Configuring Library ${TARGET_NAME}")

    message(STATUS "Adding CMake Path Argument")
    list (APPEND EXTRA_CMAKE_ARGS
      -Doatpp-swagger_INSTALL_DIR=${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Install/ThirdParty_${TARGET_NAME}/
    )
    message(STATUS "Added CMake Path Argument")

    
# Run second part of build, create target/find library we built in the above superbuild target
else()

    option(oatpp-swagger_INSTALL_DIR "Location Of oatpp-swagger Build (Used In Superbuild Process)")
    message(STATUS "${TARGET_NAME} Install Directory: ${oatpp-swagger_DIR}")


    # Set configuration for oatpp-swagger package to be found using find_package
    set(oatpp-swagger_DIR ${oatpp-swagger_INSTALL_DIR}/lib/cmake/oatpp-swagger-1.3.0/)

    # Now do the find_package stuff like normal (from the oatpp-swagger github example)
    find_package(oatpp-swagger  1.3.0 REQUIRED)
    if(oatpp-swagger_FOUND)
        message(STATUS "Found oatpp-swagger version: ${oatpp-swagger_VERSION_STRING}")
    else()
        message(FATAL_ERROR "Could not find oatpp-swagger")
    endif()

    include_directories(${oatpp-swagger_INCLUDE_DIRS})

    add_definitions( 
        -DOATPP_SWAGGER_RES_PATH="${OATPP_BASE_DIR}/bin/oatpp-swagger/res"
    )

    # Great, now this can be linked to via oatpp::oatpp-swagger, and all headers/libs are taken care of by the above code


endif()