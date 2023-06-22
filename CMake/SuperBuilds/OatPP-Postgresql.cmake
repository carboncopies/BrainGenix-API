# BG Superbuild


# Configuration Parameters
set(TARGET_NAME OatPP-Postgresql)
set(LIB_SOURCE_DIR ${LIB_DIR}/SuperBuild/oatpp-postgresql-1.3.0)


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
      -Doatpp-postgresql_INSTALL_DIR=${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Install/ThirdParty_${TARGET_NAME}/
    )
    message(STATUS "Added CMake Path Argument")

    
# Run second part of build, create target/find library we built in the above superbuild target
else()

    option(oatpp-postgresql_INSTALL_DIR "Location Of oatpp-postgresql Build (Used In Superbuild Process)")
    message(STATUS "${TARGET_NAME} Install Directory: ${oatpp-postgresql_DIR}")


    # Set configuration for oatpp-postgresql package to be found using find_package
    set(oatpp-postgresql_DIR ${oatpp-postgresql_INSTALL_DIR}/lib/cmake/oatpp-postgresql-1.3.0/)

    # Now do the find_package stuff like normal (from the oatpp-postgresql github example)
    find_package(oatpp-postgresql  1.3.0 REQUIRED)
    if(oatpp-postgresql_FOUND)
        message(STATUS "Found oatpp-Postgresql version: ${oatpp-postgresql_VERSION_STRING}")
    else()
        message(FATAL_ERROR "Could not find oatpp-postgresql")
    endif()

    include_directories(${oatpp-Postgresql_INCLUDE_DIRS})

    add_definitions( 
        -DOATPP_postgresql_RES_PATH="${OATPP_BASE_DIR}/bin/oatpp-postgresql/res"
    )

    # Great, now this can be linked to via oatpp::oatpp-postgresql, and all headers/libs are taken care of by the above code


endif()