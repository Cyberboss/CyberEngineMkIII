#!/bin/bash

echo "Removing old reports"
rm -rf HTMLCodeCoverage

echo "Downloading scripts..."
curl -O https://dimensionalpha.net/PostTestScripts.zip
unzip PostTestScripts.zip

echo "Converting test XML..."
xsltproc CTest2JUnit.xsl Testing/$(ls Testing | grep 2)/Test.xml > Test.xml

if [ $# -ne 2 ]
then
	echo "Resetting coverage counters..."
	lcov --zerocounters --directory . --base-directory . --gcov-tool Tests/llvm-gcov.sh
	lcov --capture --initial --directory . --output-file lcovrun.dat  --base-directory . --gcov-tool Tests/llvm-gcov.sh

	echo "Running test coverage..."
	Output/bin/CyberEngineMkIIITester
	
	echo "Gathering code coverage data..."
	lcov --directory . --base-directory . --gcov-tool Tests/llvm-gcov.sh --no-external --capture -o lcovrun.dat

	lcov -r lcovrun.dat *Assert.inl *Syscalls.* *.hpp -o lcovstripped.dat
	
	if [ "$(uname)" == "Darwin" ]; then
		lcov -e lcovstripped.dat *CYBPosix* *CYBLinux* -o lcov.dat
	else
		lcov -e lcovstripped.dat *CYBPosix* *CYBOSX* -o lcov.dat
	fi

	python lcov_cobertura.py lcov.dat -o Code.coveragexml

	echo "Generating html..."
	genhtml lcov.dat -o HTMLCodeCoverage --no-function-coverage

	if [ $# -eq 1 ]
	then
		echo "Publishing html to local web server"
		sudo rm -rf /srv/http
		sudo cp -r HTMLCodeCoverage/ /srv/http
		sudo cp -r HTMLStaticAnalysis /srv/http/analysis
	fi
fi