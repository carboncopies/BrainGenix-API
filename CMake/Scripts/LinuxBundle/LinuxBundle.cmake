###############################
# BG Build Versioning System #
###############################

BGBuildLogger(${Cyan} "Configuring Darwin Bundle System")

add_executable(${PROJECT_NAME} ${MAIN_SOURCES})

set(EXECUTABLE_TYPE "Binary")

BGBuildLogger(${BoldCyan} "Finished Configuring Darwin Bundle System")