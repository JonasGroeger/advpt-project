#!/bin/bash
set -o errexit

BUILD_FOLDER="_build"

if [ ! -d "$BUILD_FOLDER" ]; then
    echo "You must first build the project (chmod +x build.sh && ./build.sh)."
    exit 1
fi

if [ ! -f "$1" ]; then
	echo "Usage: ./forwardSim.sh buildlist"
	exit 2
fi

./_build/sc2sim
