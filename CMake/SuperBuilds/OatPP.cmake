# BG Superbuild


# Configuration Parameters
set(TARGET_NAME OatPP)
set(LIB_SOURCE_DIR ${LIB_DIR}/SuperBuild/oatpp-1.3.0)


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
      -Doatpp_INSTALL_DIR=${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Install/ThirdParty_${TARGET_NAME}/  
    )
    message(STATUS "Added CMake Path Argument")

    
# Run second part of build, create target/find library we built in the above superbuild target
else()

    option(oatpp_INSTALL_DIR "Location Of oatpp Build (Used In Superbuild Process)")

    message(STATUS "${TARGET_NAME} Install Directory: ${oatpp_INSTALL_DIR}")

    # set(TARGET_INCLUDES_SEARCH_PATH ${oatpp_DIR}include/oatpp-1.3.0/oatpp/oatpp/*.hpp)
    # # set(TARGET_LIBS_SEARCH_PATH ${oatpp_DIR}lib/oatpp-1.3.0/liboatpp.a)
    # message(STATUS "${TARGET_NAME} Include Search Path ${TARGET_INCLUDES_SEARCH_PATH}")
    # # message(STATUS "${TARGET_NAME} Libs Search Path ${TARGET_LIBS_SEARCH_PATH}")
    # FILE(GLOB_RECURSE TARGET_INCLUDES ${TARGET_INCLUDES_SEARCH_PATH})
    # # FILE(GLOB_RECURSE Libs ${TARGET_LIBS_SEARCH_PATH})
    # set(TARGET_LIBS ${oatpp_DIR}lib/oatpp-1.3.0/liboatpp.a)

    # add_library(oatpp ${TARGET_INCLUDES})
    # target_link_libraries(oatpp ${TARGET_LIBS})
    # set_target_properties(oatpp PROPERTIES LINKER_LANGUAGE CXX)
    # target_include_directories(oatpp PUBLIC ${oatpp_DIR}/include/oatpp-1.3.0/oatpp/)

    # Set configuration for oatpp package to be found using find_package
    set(oatpp_DIR ${oatpp_INSTALL_DIR}/lib/cmake/oatpp-1.3.0/)

    # Now do the find_package stuff like normal (from the oatpp-swagger github example)
    find_package(oatpp 1.3.0 REQUIRED)
    if(oatpp_FOUND)
      message(STATUS "Found oatpp version: ${oatpp_VERSION_STRING}")
    else()
      message(FATAL_ERROR "Could not find oatpp")
    endif()

    include_directories(${oatpp_INCLUDE_DIRS})

    # Great, now this can be linked to via oatpp::oatpp, and all headers/libs are taken care of by the above code


endif()