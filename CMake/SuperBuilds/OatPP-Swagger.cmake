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
                   -DOATPP_SWAGGER_BUILD_TESTS:BOOL=OFF

    )
    message(STATUS "Finished Configuring Library ${TARGET_NAME}")

    message(STATUS "Adding CMake Path Argument")
    list (APPEND EXTRA_CMAKE_ARGS
      -Doatpp-swagger_DIR=${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Install/ThirdParty_${TARGET_NAME}/  
    )
    message(STATUS "Added CMake Path Argument")

    
# Run second part of build, create target/find library we built in the above superbuild target
else()

    option(oatpp-swagger_DIR "Location Of oatpp Build (Used In Superbuild Process)")

    message(STATUS "${TARGET_NAME} Install Directory: ${oatpp-swagger_DIR}")

    set(TARGET_INCLUDES_SEARCH_PATH ${oatpp_DIR}include/oatpp-swagger-1.3.0/oatpp/oatpp/*.hpp)
    # set(TARGET_LIBS_SEARCH_PATH ${oatpp_DIR}lib/oatpp-1.3.0/liboatpp.a)
    message(STATUS "${TARGET_NAME} Include Search Path ${TARGET_INCLUDES_SEARCH_PATH}")
    # message(STATUS "${TARGET_NAME} Libs Search Path ${TARGET_LIBS_SEARCH_PATH}")
    FILE(GLOB_RECURSE TARGET_INCLUDES ${TARGET_INCLUDES_SEARCH_PATH})
    # FILE(GLOB_RECURSE Libs ${TARGET_LIBS_SEARCH_PATH})
    set(TARGET_LIBS ${oatpp_DIR}lib/oatpp-swagger-1.3.0/liboatpp.a)

    add_library(oatpp-swagger ${TARGET_INCLUDES})
    target_link_libraries(oatpp-swagger ${TARGET_LIBS})
    set_target_properties(oatpp-swagger PROPERTIES LINKER_LANGUAGE CXX)
    target_include_directories(oatpp-swagger PUBLIC ${oatpp_DIR}/include/oatpp-swagger-1.3.0/oatpp-swagger/)

endif()