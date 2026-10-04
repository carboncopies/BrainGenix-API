# Erase Existing Deb Builds From Artifacts Dir
rm -f ../Artifacts/*.deb

# Enter Artifact/Binary Dir
cd ..
cd Build

# Run CPack (on failure, show the log CPack points at; it isn't kept as an artifact)
if ! cpack; then
    cat _CPack_Packages/*/*/PreinstallOutput.log 2>/dev/null
    exit 1
fi