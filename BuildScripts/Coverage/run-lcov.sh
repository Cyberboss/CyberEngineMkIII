#!/bin/bash
#run from project root
echo "Gathering code coverage data..."
lcov --directory . --base-directory . --gcov-tool BuildScripts/Coverage/llvm-gcov.sh --no-external --capture -o lcovrun.dat --rc lcov_branch_coverage=1 --rc geninfo_gcov_all_blocks=1

lcov --directory . --base-directory . -r lcovrun.dat *Assert.inl *Syscalls.* *.hpp -o lcovstripped.dat --rc lcov_branch_coverage=1 --rc geninfo_gcov_all_blocks=1 --rc genhtml_branch_coverage=1 --gcov-tool BuildScripts/Coverage/llvm-gcov.sh 
	
if [ "$(uname)" == "Darwin" ]; then
	lcov --directory . --base-directory . -e lcovstripped.dat *CYBPosix* *CYBLinux* -o lcov.info --rc lcov_branch_coverage=1 --rc geninfo_gcov_all_blocks=1 --gcov-tool BuildScripts/Coverage/llvm-gcov.sh 
	bash <(curl -s https://codecov.io/bash) -X gcov -f lcov.info -F OSX
else
	lcov --directory . --base-directory . -e lcovstripped.dat *CYBPosix* *CYBOSX* -o lcov.info --rc lcov_branch_coverage=1 --rc geninfo_gcov_all_blocks=1 --gcov-tool BuildScripts/Coverage/llvm-gcov.sh 
	bash <(curl -s https://codecov.io/bash) -X gcov -f lcov.info -F Linux
fi
