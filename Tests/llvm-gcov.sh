#!/bin/bash

if [[ "$(uname)" == "Darwin" ]]; then
	exec /Library/Developer/CommandLineTools/usr/bin/llvm-cov gcov "$@"
else
	exec llvm-cov gcov "$@"
fi