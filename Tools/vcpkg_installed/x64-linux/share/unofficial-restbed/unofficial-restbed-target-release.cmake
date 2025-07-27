#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "unofficial::restbed::restbed-static" for configuration "Release"
set_property(TARGET unofficial::restbed::restbed-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(unofficial::restbed::restbed-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/librestbed.a"
  )

list(APPEND _cmake_import_check_targets unofficial::restbed::restbed-static )
list(APPEND _cmake_import_check_files_for_unofficial::restbed::restbed-static "${_IMPORT_PREFIX}/lib/librestbed.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
