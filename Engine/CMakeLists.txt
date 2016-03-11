cmake_minimum_required (VERSION 3.4.3)

project(Engine)

include_directories(${CYB_BASE_DIR}/Engine)

IF(APPLE)
set(PlatformImplentations "")
ELSE()
set(PlatformImplentations "")
ENDIF()

add_library(EngineBase STATIC

#Platform
${PlatformImplementations}

CYBPosixSyscalls.cpp
CYBPosixProcess.cpp

CYBProcess.cpp

#Engine
CYBMain.cpp
)

add_precompiled_header(EngineBase CYB.cpp)

set_property(TARGET EngineBase PROPERTY CXX_STANDARD 14)
set_property(TARGET EngineBase PROPERTY CXX_STANDARD_REQUIRED ON)