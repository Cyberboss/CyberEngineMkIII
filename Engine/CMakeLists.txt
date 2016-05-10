cmake_minimum_required (VERSION 3.4.3)

include_directories(${CYB_BASE_DIR}/Engine)

IF(APPLE)
set(PlatformImplentations "CYBOSXThread.cpp")
ELSE()
set(PlatformImplentations "CYBLinuxThread.cpp")
ENDIF()

add_library(EngineBase STATIC

${PlatformImplementations}

#Posix
CYBPosixSyscalls.cpp
CYBPosixThread.cpp
CYBPosixProcess.cpp
CYBPosixVirtualMemory.cpp
CYBPosixModule.cpp
CYBPosixMutex.cpp

#Platform
CYBThread.cpp
CYBProcess.cpp
CYBModule.cpp

#Engine

#Memory
CYBBlock.cpp
CYBLargeBlock.cpp
CYBHeap.cpp

#Core
CYBException.cpp
CYBCore.cpp
)

set_property(TARGET EngineBase PROPERTY CXX_STANDARD 14)
set_property(TARGET EngineBase PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(EngineBase dl)

set_property (SOURCE "CYB.cpp" PROPERTY COTIRE_DEPENDENCY "TRUE")
set_target_properties(EngineBase PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "CYB.hpp")
cotire(EngineBase)