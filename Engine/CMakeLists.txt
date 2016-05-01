cmake_minimum_required (VERSION 3.4.3)

include_directories(${CYB_BASE_DIR}/Engine)

IF(APPLE)
set(PlatformImplentations "CYBOSXThread.cpp")
ELSE()
set(PlatformImplentations "CYBLinuxThread.cpp")
ENDIF()

add_library(EngineBase STATIC

#Platform
${PlatformImplementations}

CYBPosixSyscalls.cpp
CYBPosixThread.cpp
CYBPosixProcess.cpp
CYBPosixVirtualMemory.cpp
CYBPosixModule.cpp

CYBException.cpp
CYBThread.cpp
CYBProcess.cpp
CYBModule.cpp

#Engine
CYBCore.cpp
)

set_property(TARGET EngineBase PROPERTY CXX_STANDARD 14)
set_property(TARGET EngineBase PROPERTY CXX_STANDARD_REQUIRED ON)

set_property (SOURCE "CYB.cpp" PROPERTY COTIRE_DEPENDENCY "TRUE")
set_target_properties(EngineBase PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "CYB.hpp")
cotire(EngineBase)