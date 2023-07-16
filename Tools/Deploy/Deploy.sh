#!/bin/bash

# echo "Downloading Latest BrainGenix API Package"
# wget -O Artifacts.zip https://gitlab.braingenix.org/api/v4/projects/BrainGenix-API/jobs/artifacts/master/download?job=Ubuntu_2204_Package

# echo "Unzipping Artifacts"

echo "Finding Current Package Version"
DEBIAN_FRONTEND=noninteractive
cd $Artifacts && PackageVar=$(find -type f -name 'BrainGenix-API*.*.deb');
echo "Found API Package At $PackageVar"

echo "Uninstalling Current Package"
sudo apt remove braingenix-api

echo "Installing New Package"
sudo apt install $PackageVar