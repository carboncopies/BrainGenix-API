#!/bin/bash

cd ..
mkdir Build
cd Build

./../ThirdParty/vcpkg/bootstrap-vcpkg.sh

cmake -B build -S ..
cmake --build build