##########################
# Builds Library For BG #
##########################

BGBuildLogger(${Green} "Configuring Backward Library")
if (${ENABLE_BACKWARD})
    add_subdirectory(${LIB_DIR}/NonSuperBuild/backward)
    BGBuildLogger(${Green} "Backwards Library Enabled, Configuring")
else()
    set(BACKWARD_ENABLE "")
    BGBuildLogger(${Green} "Backwards Library Disabled, Skipping")
endif()
BGBuildLogger(${BoldGreen} "Finished Configuring Backward Library")