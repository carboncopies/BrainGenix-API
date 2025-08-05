#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "unofficial::libmariadb" for configuration "Debug"
set_property(TARGET unofficial::libmariadb APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(unofficial::libmariadb PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/debug/lib/libmariadb.a"
  )

list(APPEND _cmake_import_check_targets unofficial::libmariadb )
list(APPEND _cmake_import_check_files_for_unofficial::libmariadb "${_IMPORT_PREFIX}/debug/lib/libmariadb.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
