#!/bin/bash

# Enter Artifact/Binary Dir
cd ..
cd Binaries

# Function to handle the SIGINT signal (Ctrl+C)
handle_sigint() {
    echo "Exiting script..."
    exit 0
}

# Trap the SIGINT signal and call the handle_sigint function
trap handle_sigint SIGINT

# Depending On Platform, Run
while true
do
    ./BrainGenix-API $1 $2 $3 $4
    # Add a 1-second delay before restarting
    sleep 1
done
