#!/bin/bash

# echo "Downloading Latest BrainGenix API Package"
# wget -O Artifacts.zip https://gitlab.braingenix.org/api/v4/projects/BrainGenix-API/jobs/artifacts/master/download?job=Ubuntu_2204_Package

# echo "Unzipping Artifacts"

echo "-- Finding Current Package Version"
cd $Artifacts && PackageVar=$(find -type f -name 'BrainGenix-API*.*.deb');
echo "Found API Package At $PackageVar"

echo "-- Setting Noninteractive Mode For APT"
DEBIAN_FRONTEND=noninteractive

# echo "-- Uninstalling Current Package"
# sudo --preserve-env apt remove braingenix-api -y

echo "-- Installing New Package"
sudo --preserve-env apt install $PackageVar -y --reinstall