# Superbuild Main Runner

# Setup System
include (ExternalProject)

set_property (DIRECTORY PROPERTY EP_BASE Dependencies)

set (DEPENDENCIES)
set (EXTRA_CMAKE_ARGS)


# Include Superbuilds
# include(${CMAKE_UTILS_DIR}/SuperBuilds/Boost.cmake)

include(${CMAKE_UTILS_DIR}/SuperBuilds/YAMLCPP.cmake)
include(${CMAKE_UTILS_DIR}/SuperBuilds/OatPP.cmake)
include(${CMAKE_UTILS_DIR}/SuperBuilds/OatPP-Swagger.cmake)
include(${CMAKE_UTILS_DIR}/SuperBuilds/OatPP-Postgresql.cmake)
include(${CMAKE_UTILS_DIR}/SuperBuilds/API.cmake)

