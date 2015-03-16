#!/bin/bash
set -o errexit

BUILD_FOLDER="build"

# Remove left-overs from last build
rm -rf $BUILD_FOLDER

# Create build directory
mkdir -p $BUILD_FOLDER
cd $BUILD_FOLDER

cmake ..
make -j4
