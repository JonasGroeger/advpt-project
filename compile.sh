#!/bin/bash
set -o errexit

BUILD_FOLDER="build"

# Create build directory
mkdir -p $BUILD_FOLDER
cd $BUILD_FOLDER

cmake ..
make -j4
