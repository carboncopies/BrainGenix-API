#!/bin/bash

# Navigate to the project root (one level up from Tools)
echo "Navigating to the project root"
cd .. || { echo "Failed to navigate to the project root"; exit 1; }

# # Activate the virtual environment
# echo "Activating virtual environment"
# source venv/bin/activate || { echo "Failed to activate virtual environment"; exit 1; }

# # Verify neuroglancer is installed
# echo "Checking for neuroglancer in the virtual environment"
# pip list | grep neuroglancer || { echo "neuroglancer is not installed in the virtual environment"; exit 1; }

# Enter Artifact/Binary Dir
echo "Changing directory to Binaries"
cd Binaries || { echo "Failed to change directory to Binaries"; exit 1; }

# Depending On Platform, Run
echo "Running BrainGenix-API with arguments: $1 $2 $3 $4"
./BrainGenix-API "$1" "$2" "$3" "$4" || { echo "Failed to run BrainGenix-API"; exit 1; }

# Deactivate the virtual environment (optional)
# deactivate
