#!/bin/bash

docker tag api_build_linux_debian_10 10.1.3.11:5000/api_build_linux_debian_10
docker push 10.1.3.11:5000/api_build_linux_debian_10