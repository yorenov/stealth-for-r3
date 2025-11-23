#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "rakhook::rakhook" for configuration "Debug"
set_property(TARGET rakhook::rakhook APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(rakhook::rakhook PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/rakhook.lib"
  )

list(APPEND _cmake_import_check_targets rakhook::rakhook )
list(APPEND _cmake_import_check_files_for_rakhook::rakhook "${_IMPORT_PREFIX}/lib/rakhook.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
