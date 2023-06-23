###############################
# BG Build Versioning System #
###############################

BGBuildLogger(${Cyan} "Configuring Linux Bundle System")

add_executable(${PROJECT_NAME} ${MAIN_SOURCES})

set(EXECUTABLE_TYPE "Binary")

BGBuildLogger(${BoldCyan} "Finished Configuring Linux Bundle System")