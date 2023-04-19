
# Enter Artifact/Binary Dir
echo "[BG BUILD SYSTEM] - Entering Artifacts Directory"
cd ..
cd Binaries

# Detect Platform, Execute Program From That
echo "[BG BUILD SYSTEM] - Detecting Platform"
MachineName="$(uname -s)"
case "${MachineName}" in
    Linux*)     Machine="Linux";;
    Darwin*)    Machine="Mac";;
    CYGWIN*)    Machine="Cygwin";;
    MINGW*)     Machine="MinGw";;
    *)          Machine="UNKNOWN:${MachineName}"
esac
echo "[BG BUILD SYSTEM] - Detected Platform To Be '${Machine}'"


# Depending On Platform, Run
echo "[BG BUILD SYSTEM] - Running Binary"
if [ ${Machine} == "Linux" ]
then
    ./BrainGenix-API
elif ${Machine} == "Mac"
then
    open BrainGenix-API.app
else
    echo "Unknown Platform, Cannot Run Executable"
fi
