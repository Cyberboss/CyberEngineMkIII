#!/bin/bash

#override the clag version we set for linux
if [ "$(uname)" == "Darwin" ]; then
	$CXXCOMPILER=clang++
fi

if [ "${TRAVIS_PULL_REQUEST}" = "false" ]; then
	cmake . -DCMAKE_CXX_COMPILER=$CXXOMPILER -DCMAKE_BUILD_TYPE=Release -DCOTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES=1 -DGOLD_PLUGIN=OFF
else
	cmake . -DCMAKE_CXX_COMPILER=$CXXOMPILER -DCMAKE_BUILD_TYPE=Debug -DCOTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES=1 -DGOLD_PLUGIN=OFF
fi
make
ctest