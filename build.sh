#!/bin/bash
set -o errexit

BUILD_FOLDER="build"
HOSTNAME=$(hostname -f)

# Remove left-overs from last build
rm -rf $BUILD_FOLDER

# Create build directory
mkdir -p $BUILD_FOLDER
cd $BUILD_FOLDER

# Create the Makefile
# if we are in the cip pool, clang compiler has a non default location...
# do all cip clients have a hostname starting with "fau"? i think so
if [[ ${HOSTNAME:0:3} = "fau" ]] 
then
	cmake -DCMAKE_CXX_COMPILER=/local/clang-3.5/bin/clang++ ..
else 
	cmake ..
fi

make
