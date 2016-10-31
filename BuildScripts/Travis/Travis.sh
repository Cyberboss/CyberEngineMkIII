#!/bin/bash

#override the clag version we set for linux
if [ "$(uname)" == "Darwin" ]; then
	export CXXCOMPILER=clang++
	export CCOMPILER=clang
fi

if [ "${BUILD_RELEASE}" = "true" ]; then
	echo 'Building release version'
	cmake . -DCMAKE_C_COMPILER=$CCOMPILER -DCMAKE_CXX_COMPILER=$CXXOMPILER -DCMAKE_BUILD_TYPE=Release -DCOTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES=1 -DGOLD_PLUGIN=OFF
else
	echo 'Building debug version'
	cmake . -DCMAKE_C_COMPILER=$CCOMPILER -DCMAKE_CXX_COMPILER=$CXXOMPILER -DCMAKE_BUILD_TYPE=Debug -DCOTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES=1 -DGOLD_PLUGIN=OFF
fi
make
ctest