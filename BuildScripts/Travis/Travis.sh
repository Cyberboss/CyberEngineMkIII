#!/bin/bash

#override the clag version we set for linux
if [ "$(uname)" == "Darwin" ]; then
	echo 'Detected OS: X'
	if [ "${BUILD_RELEASE}" = "true" ]; then
		echo 'Building release version'
		cmake . -DCMAKE_BUILD_TYPE=Release -DCOTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES=1
	else
		echo 'Building debug version'
		cmake . -DCMAKE_BUILD_TYPE=Debug -DCOTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES=1
	fi
else
	echo 'Detected OS: Linux'
	if [ "${BUILD_RELEASE}" = "true" ]; then
		echo 'Building release version'
		cmake . -DCMAKE_C_COMPILER=$CCOMPILER -DCMAKE_CXX_COMPILER=$CXXOMPILER -DCMAKE_BUILD_TYPE=Release -DCOTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES=1 -DGOLD_PLUGIN=OFF
	else
		echo 'Building debug version'
		cmake . -DCMAKE_C_COMPILER=$CCOMPILER -DCMAKE_CXX_COMPILER=$CXXOMPILER -DCMAKE_BUILD_TYPE=Debug -DCOTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES=1 -DGOLD_PLUGIN=OFF
	fi
fi
cmake --build . && ctest -V