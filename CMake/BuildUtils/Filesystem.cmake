##########################
# Builds Library For BG #
##########################

BGBuildLogger(${Green} "Configuring Filesystem Library")
add_subdirectory(${LIB_DIR}/NonSuperBuild/filesystem)
# disable_target_warnings(ghc_filesystem)
BGBuildLogger(${BoldGreen} "Finished Configuring Filesystem Library")