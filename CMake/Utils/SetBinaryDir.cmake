##################
# BG BUILD UTIL #
##################

BGBuildLogger(${Green} "Setting Binary Directories")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}/lib)

# foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
#     string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
#     set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BIN_DIR} )
#     set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BIN_DIR} )
#     set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BIN_DIR} )
# endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

set(CMAKE_INSTALL_PREFIX ${BIN_DIR})
BGBuildLogger(${BoldGreen} "Finished Setting Binary Directories")