#!/bin/bash
#run from project root
curl -O -L https://github.com/linux-test-project/lcov/archive/v1.12.tar.gz
gunzip v1.12.tar.gz
tar -xvf v1.12.tar
cd lcov-1.12
#cant do this on osx due to https://github.com/linux-test-project/lcov/issues/19#issuecomment-257070373
#sudo make install

#brute force it instead
sudo cp bin/* /usr/local/bin/

cd ..
rm -r v1.12.tar v1.12.tar.gz lcov-1.12

lcov --directory . --base-directory . --gcov-tool BuildScripts/Coverage/llvm-gcov.sh --no-external --capture -o lcovrun.dat

lcov --directory . --base-directory . -r lcovrun.dat *Assert.inl *Syscalls.* *.hpp -o lcovstripped.dat --gcov-tool BuildScripts/Coverage/llvm-gcov.sh 
	
if [ "$(uname)" != "Darwin" ]; then
	lcov --directory . --base-directory . -e lcovstripped.dat *CYBPosix* *CYBLinux* -o lcov.info --gcov-tool BuildScripts/Coverage/llvm-gcov.sh 
	bash <(curl -s https://codecov.io/bash) -X gcov -f lcov.info -F Linux
else
	lcov --directory . --base-directory . -e lcovstripped.dat *CYBPosix* *CYBOSX* -o lcov.info --gcov-tool BuildScripts/Coverage/llvm-gcov.sh 
	bash <(curl -s https://codecov.io/bash) -X gcov -f lcov.info -F OSX
fi
