#!/bin/bash

docker tag api_build_linux_ubuntu_2204 10.1.3.11:5000/api_build_linux_ubuntu_2204
docker push 10.1.3.11:5000/api_build_linux_ubuntu_2204