# CMake file for shared BG code
# This just appends to the source list

list(APPEND MAIN_SOURCES
    ${SRC_DIR}/Shared/Logger/Config.cpp
    ${SRC_DIR}/Shared/Logger/Config.h
    ${SRC_DIR}/Shared/Logger/LoggingSystem.cpp
    ${SRC_DIR}/Shared/Logger/LoggingSystem.h
)