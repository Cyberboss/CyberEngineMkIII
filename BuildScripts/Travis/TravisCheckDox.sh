#!/bin/bash

#Check all 3 platforms for dox errors
mkdir TestDocs
((cat 'BuildScripts/Doxygen/CyberEngineMkIIIDoxy.cfg' ; echo 'EXPAND_AS_DEFINED = DEBUG TARGET_OS_WINDOWS' ; echo 'OUTPUT_DIRECTORY = TestDocs'; echo 'EXCLUDE_PATTERNS = CYBPosix* CYBLinux* CYBOSX*') | doxygen 2>&1) | tee doxygen.log

if grep -q -i "warning" "doxygen.log"; then
	echo 'Warnings detected in documentation. Failing build!'
	exit 1
 fi
 
((cat 'BuildScripts/Doxygen/CyberEngineMkIIIDoxy.cfg' ; echo 'EXPAND_AS_DEFINED = DEBUG TARGET_OS_MAC' ; echo 'OUTPUT_DIRECTORY = TestDocs'; echo 'EXCLUDE_PATTERNS = CYBWin32* CYBOSX*') | doxygen 2>&1) | tee doxygen.log

if grep -q -i "warning" "doxygen.log"; then
	echo 'Warnings detected in documentation. Failing build!'
	exit 1
 fi
 
((cat 'BuildScripts/Doxygen/CyberEngineMkIIIDoxy.cfg' ; echo 'EXPAND_AS_DEFINED = DEBUG TARGET_OS_LINUX' ; echo 'OUTPUT_DIRECTORY = TestDocs'; echo 'EXCLUDE_PATTERNS = CYBLinux* CYBWin32*') | doxygen 2>&1) | tee doxygen.log

if grep -q -i "warning" "doxygen.log"; then
	echo 'Warnings detected in documentation. Failing build!'
	exit 1
 fi