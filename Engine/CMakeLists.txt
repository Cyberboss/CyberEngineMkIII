cmake_minimum_required (VERSION 3.4.3)

include_directories(${CYB_BASE_DIR}/Engine)

IF(APPLE)
set(PlatformImplentations "CYBOSXThread.cpp")
ELSE()
set(PlatformImplentations "CYBLinuxThread.cpp")
ENDIF()

add_library(EngineBase STATIC

CYB.cpp
CYB.hpp

#Platform
${PlatformImplementations}

CYBPosixSyscalls.cpp
CYBPosixThread.cpp
CYBPosixProcess.cpp
CYBPosixVirtualMemory.cpp

CYBException.cpp
CYBThread.cpp
CYBProcess.cpp

#Engine
CYBInit.cpp
)

add_precompiled_header(EngineBase CYB.hpp FORCEINCLUDE)

set_property(TARGET EngineBase PROPERTY CXX_STANDARD 14)
set_property(TARGET EngineBase PROPERTY CXX_STANDARD_REQUIRED ON)