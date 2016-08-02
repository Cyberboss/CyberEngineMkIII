#!/bin/bash

if [[ "$(uname)" == "Darwin" ]]; then
	exec /Library/Developer/CommandLineTools/usr/bin/llvm-cov gcov -a "$@"
else
	exec llvm-cov gcov -a "$@"
fi