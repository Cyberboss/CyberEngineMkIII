#!/bin/bash

echo "Downloading scripts..."
curl -O https://dimensionalpha.net/PostTestScripts.zip
unzip PostTestScripts.zip

echo "Converting test XML..."
xsltproc CTest2JUnit.xsl Testing/$(ls Testing | grep 2)/Test.xml > Test.xml

if [ $# -ne 2 ]
then
	echo "Gathering code coverage data..."
	lcov --directory . --base-directory . --gcov-tool Tests/llvm-gcov.sh --no-external --capture -o lcovrun.dat
	lcov -r lcovrun.dat *Assert.inl *CYBSyscalls.inl *.hpp -o lcov.dat
	python lcov_cobertura.py lcov.dat -o Code.coveragexml

	echo "Generating html..."
	genhtml lcov.dat -o HTMLCodeCoverage

	if [ $# -eq 1 ]
	then
		echo "Publishing html to local web server"
		sudo rm -rf /srv/http
		sudo cp -r HTMLCodeCoverage/ /srv/http
	fi
fi