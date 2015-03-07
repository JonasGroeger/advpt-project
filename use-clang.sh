#!/bin/bash
set -o errexit

HOSTNAME=$(hostname -f)

export CC=`which clang`
export CXX=`which clang++`

# CIP
if [[ ${HOSTNAME:0:3} = "fau" ]]; then
    export CC=/local/clang-3.5/bin/clang
    export CXX=/local/clang-3.5/bin/clang++
fi
