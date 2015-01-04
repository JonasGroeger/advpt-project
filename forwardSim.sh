#!/bin/bash
set -o errexit

BUILD_FOLDER="build"

if [ ! -d "$BUILD_FOLDER" ]; then
    echo "You must first build the project (chmod +x build.sh && ./build.sh)."
    exit 1
fi

./$BUILD_FOLDER/sc2sim "$@"
