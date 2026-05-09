#!/bin/bash

# Get Into Root Repo Dir
echo "[BG BUILD HELPER] - Entering Root Repo Directory"
cd ..


# Configure Build Type and Jobs
BuildType="Debug"
JOBS=""

while [[ $# -gt 0 ]]; do
  case $1 in
    -j|--jobs)
      JOBS="$2"
      shift # past argument
      shift # past value
      ;;
    Debug|Release|RelWithDebInfo|MinSizeRel)
      BuildType="$1"
      shift # past value
      ;;
    *)
      # Assume first unknown is jobs if it's a number
      if [[ $1 =~ ^[0-9]+$ ]]; then
        JOBS="$1"
      fi
      shift
      ;;
  esac
done

if [ -z "$JOBS" ]; then
    if [ "$(uname)" = "Darwin" ]; then
        JOBS=$(sysctl -n hw.ncpu 2>/dev/null || echo 4)
    else
        JOBS=$(nproc 2>/dev/null || echo 4)
    fi
fi

echo "[BG BUILD HELPER] - Building in $BuildType mode with $JOBS jobs."

# Delete Binary If Exists 
echo "[BG BUILD HELPER] - Checking If Binary Exists"
if [ -f "Binaries/BrainGenix-API" ]
then
    echo "[BG BUILD HELPER] - Removing Last Executable"
    rm Binaries/BrainGenix-API
else
    echo "[BG BUILD HELPER] - No Binaries To Clean"
fi

# Check If Build Type Correct
if grep -q $BuildType "Build/BuildType"
then
    echo "[BG BUILD HELPER] - Detected Matching Build Type"
else
    echo "[BG BUILD HELPER] - Build Type Mismatch, Cleaning First"
    rm -rf Build/
fi


# Check If Configuration Needs To Be Run
echo "[BG BUILD HELPER] - Checking If Build Directory Already Exists"
if [ -d "Build" ]
then 
    echo "[BG BUILD HELPER] - Build Directory Already Exists, Skipping Generation"
    cd Build
else 
    # Create Build Dir
    echo "[BG BUILD HELPER] - Creating Build Directory"
    mkdir -p Build

    # Enter Build Dir
    echo "[BG BUILD HELPER] - Entering Build Directory"
    cd Build

    # Make Only BrainGenix-API
    echo "[BG BUILD HELPER] - Configuring Build Files"
    if [ "$(uname)" = "Darwin" ]; then
        # Resolve paths
        NINJA_PATH=$(which ninja)
        if [ -z "$NINJA_PATH" ]; then
            echo "[BG BUILD HELPER] - Error: ninja not found in PATH"
            exit 1
        fi
        CXX_PATH=$(which clang++)
        SDK_PATH=$(xcrun --show-sdk-path)

        # Keep old vcpkg ports buildable with newer CMake on macOS.
        export CMAKE_POLICY_VERSION_MINIMUM=3.5
        export VCPKG_KEEP_ENV_VARS=CMAKE_POLICY_VERSION_MINIMUM

        cmake .. -G "Ninja" \
            -DCMAKE_MAKE_PROGRAM="$NINJA_PATH" \
            -DCMAKE_CXX_COMPILER="$CXX_PATH" \
            -DCMAKE_OSX_SYSROOT="$SDK_PATH" \
            -DCMAKE_BUILD_TYPE="$BuildType" \
            -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    else
        cmake .. -D CMAKE_BUILD_TYPE="$BuildType"
    fi

    # Set Config Var
    echo "[BG BUILD HELPER] - Saving Build Type Configuration Of $BuildType"
    echo "$BuildType" > "BuildType"
fi


# Build Files
echo "[BG BUILD HELPER] - Building, Please Wait. This may take some time"
cmake --build . -j $JOBS

# Return status code of build (0=success, else=fail)
exit $?
