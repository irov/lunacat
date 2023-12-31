#!/bin/bash

if test -f "/usr/local/bin/cmake"; then
    CMAKE=/usr/local/bin/cmake
fi

if test -f "/Applications/CMake.app/Contents/bin/cmake"; then
    CMAKE=/Applications/CMake.app/Contents/bin/cmake
fi

CONFIGURATION=$1

mkdir -p ../../solutions/solution_xcode/$CONFIGURATION
pushd ../../solutions/solution_xcode/$CONFIGURATION
$CMAKE -G"Xcode" "$PWD/../../../cmake/lunacat_xcode" -DCMAKE_BUILD_TYPE:STRING=$CONFIGURATION -DCMAKE_CONFIGURATION_TYPES:STRING="$CONFIGURATION" -DCMAKE_OSX_DEPLOYMENT_TARGET=10.9
popd
