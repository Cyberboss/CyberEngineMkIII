#!/bin/bash

echo "Downloading scripts..."
curl -O https://dimensionalpha.net/PostTestScripts.zip
unzip PostTestScripts.zip

echo "Converting test XML..."
xsltproc CTest2JUnit.xsl Testing/$(ls Testing | grep 2)/Test.xml > Test.xml

echo "Gathering code coverage data..."
lcov --directory . --base-directory . --gcov-tool Tests/llvm-gcov.sh --no-external --capture -o lcovrun.dat
lcov -r lcovrun.dat *Assert.inl *CYBSyscalls.inl *.hpp -o lcov.dat
python lcov_cobertura.py lcov.dat -o Code.coveragexml

if [ $# -eq 1 ]
then
	echo "Generating html..."
	genhtml lcov.dat -o HTMLCodeCoverage
	sudo rm -rf /srv/http
	sudo cp -r HTMLCodeCoverage/ /srv/http
fi
