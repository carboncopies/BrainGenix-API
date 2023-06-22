# BG Superbuild


# Configuration Parameters
set(TARGET_NAME OatPP)
set(LIB_SOURCE_DIR ${LIB_DIR}/SuperBuild/oatpp)


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

    )
    message(STATUS "Finished Configuring Library ${TARGET_NAME}")

    message(STATUS "Adding CMake Path Argument")
    list (APPEND EXTRA_CMAKE_ARGS
      -Doatpp_DIR=${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Install/ThirdParty_${TARGET_NAME}/  
    )
    message(STATUS "Added CMake Path Argument")

    
# Run second part of build, create target/find library we built in the above superbuild target
else()

    option (${oatpp_DIR} "Location Of oatpp Build (Used In Superbuild Process)")

    message(STATUS "FreeImage Install Directory: ${oatpp_DIR}")
    FILE(GLOB_RECURSE Includes ${oatpp_DIR}*/*.h)
    FILE(GLOB_RECURSE Libs ${oatpp_DIR}*/*.a)
    message(STATUS ${Includes})
    add_library(oatpp ${Includes})
    target_link_libraries(oatpp ${Libs})
    set_target_properties(oatpp PROPERTIES LINKER_LANGUAGE CXX)
    target_include_directories(oatpp PUBLIC ${oatpp_DIR}/include)

endif()