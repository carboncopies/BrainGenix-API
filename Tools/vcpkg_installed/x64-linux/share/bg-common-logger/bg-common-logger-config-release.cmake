#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "bg-common-logger" for configuration "Release"
set_property(TARGET bg-common-logger APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(bg-common-logger PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${CMAKE_CURRENT_LIST_DIR}/../../lib/libbg-common-logger.a"
  )

list(APPEND _cmake_import_check_targets bg-common-logger )
list(APPEND _cmake_import_check_files_for_bg-common-logger "${CMAKE_CURRENT_LIST_DIR}/../../lib/libbg-common-logger.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
