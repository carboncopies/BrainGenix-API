#!/bin/bash

echo "Downloading Latest BrainGenix API Package"
wget -O Artifacts.zip https://gitlab.braingenix.org/api/v4/projects/BrainGenix-API/jobs/artifacts/master/download?job=Ubuntu_2204_Package

echo "Unzipping Artifacts"

echo "Uninstalling Current Package"

echo "Installing New Package"