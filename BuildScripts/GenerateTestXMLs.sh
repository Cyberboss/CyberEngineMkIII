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
	echo "Gathering code coverage data..."
	lcov --directory . --base-directory . --gcov-tool Tests/llvm-gcov.sh --no-external --capture -o lcovrun.dat --rc lcov_branch_coverage=1  --rc geninfo_gcov_all_blocks=1 --rc genhtml_branch_coverage=1

	lcov --directory . --base-directory . -r lcovrun.dat *Assert.inl *Syscalls.* *.hpp -o lcovstripped.dat --rc lcov_branch_coverage=1 --rc geninfo_gcov_all_blocks=1 --rc genhtml_branch_coverage=1 --gcov-tool Tests/llvm-gcov.sh 
	
	if [ "$(uname)" == "Darwin" ]; then
		lcov --directory . --base-directory . -e lcovstripped.dat *CYBPosix* *CYBLinux* -o lcov.dat --rc lcov_branch_coverage=1 --rc geninfo_gcov_all_blocks=1 --rc genhtml_branch_coverage=1 --gcov-tool Tests/llvm-gcov.sh 
	else
		lcov --directory . --base-directory . -e lcovstripped.dat *CYBPosix* *CYBOSX* -o lcov.dat --rc lcov_branch_coverage=1 --rc geninfo_gcov_all_blocks=1 --rc genhtml_branch_coverage=1 --gcov-tool Tests/llvm-gcov.sh 
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