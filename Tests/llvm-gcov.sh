#!/bin/bash

if [[ "$unamestr" == 'Linux' ]]; then
	exec llvm-cov gcov "$@"
else
	exec /Library/Developer/CommandLineTools/usr/bin/llvm-cov gcov "$@"
fi