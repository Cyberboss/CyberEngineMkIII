cmake_minimum_required (VERSION 3.4.3)

include_directories(${CYB_BASE_DIR}/Engine)

find_package(Threads REQUIRED)

IF(APPLE)
set(PlatformImplentations "Platform/OSX/CYBOSXThread.cpp")
ELSE()
set(PlatformImplentations "Platform/Linux/CYBLinuxThread.cpp")
ENDIF()

add_library(EngineBase STATIC

${PlatformImplementations}

#Posix
Platform/Posix/CYBPosixSyscalls.cpp
Platform/Posix/CYBPosixThread.cpp
Platform/Posix/CYBPosixProcess.cpp
Platform/Posix/CYBPosixVirtualMemory.cpp
Platform/Posix/CYBPosixModule.cpp
Platform/Posix/CYBPosixMutex.cpp

#Platform
Platform/CYBThread.cpp
Platform/CYBProcess.cpp
Platform/Modules/CYBModule.cpp

#Engine

#Memory
Engine/Memory/CYBBlock.cpp
Engine/Memory/CYBLargeBlock.cpp
Engine/Memory/CYBHeap.cpp

#Core
Engine/Core/CYBException.cpp
Engine/Core/CYBCore.cpp
)

set_property(TARGET EngineBase PROPERTY CXX_STANDARD 14)
set_property(TARGET EngineBase PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(EngineBase dl)

set_property (SOURCE "CYB.cpp" PROPERTY COTIRE_DEPENDENCY "TRUE")
set_target_properties(EngineBase PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "CYB.hpp")
cotire(EngineBase)