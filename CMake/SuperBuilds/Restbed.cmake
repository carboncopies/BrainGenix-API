# BG Superbuild


# Configuration Parameters
set(TARGET_NAME RestBed)
set(LIB_SOURCE_DIR ${LIB_DIR}/SuperBuild/restbed)



# First part of two, build the library if we're currently doing so
if (USE_SUPERBUILD)

    # Add To Dependencies
    list (APPEND DEPENDENCIES ThirdParty_${TARGET_NAME})

    # copy src to build dir
    # message(STATUS "Copying Source Dir For ${TARGET_NAME} To Build Dir")
    # file(COPY ${LIB_SOURCE_DIR}/restbed DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Source/Restbed)
    # message(STATUS "Done Copying ${TARGET_NAME}")

    # Create External Project
    message(STATUS "Configuring Library ${TARGET_NAME}")
    ExternalProject_Add (ThirdParty_${TARGET_NAME}

        SOURCE_DIR ${LIB_SOURCE_DIR}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Install/ThirdParty_${TARGET_NAME}/
        # PATCH_COMMAND cd dependency/openssl && ./config && make -j && cd ../.. && cmake .

    )
    message(STATUS "Finished Configuring Library ${TARGET_NAME}")

    message(STATUS "Adding CMake Path Argument")
    list (APPEND EXTRA_CMAKE_ARGS
      -Drestbed_DIR=${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Install/ThirdParty_${TARGET_NAME}/  
    )
    message(STATUS "Added CMake Path Argument")

    
# Run second part of build, create target/find library we built in the above superbuild target
else()

    option (${restbed_DIR} "Location Of meshoptimizer Build (Used In Superbuild Process)")

    message(STATUS "FreeImage Install Directory: ${yaml-cpp_DIR}")
    FILE(GLOB_RECURSE Includes ${restbed_DIR}*/*.h)
    FILE(GLOB_RECURSE Libs ${restbed_DIR-cpp_DIR}*/*.a)
    message(STATUS ${Includes})
    add_library(${TARGET_NAME} ${Includes})
    target_link_libraries(${TARGET_NAME} ${Libs})
    set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)
    target_include_directories(${TARGET_NAME} PUBLIC ${restbed_DIR}/include)

endif()