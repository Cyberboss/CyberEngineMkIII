cmake_minimum_required (VERSION 3.4.3)

include_directories(${CYB_BASE_DIR}/Engine)

find_package(Threads REQUIRED)

add_library(EngineBase STATIC


#Platform

#System
Platform/System/CYBThread.cpp
Platform/System/CYBProcess.cpp

#Posix
Platform/System/Posix/CYBPosixSyscalls.cpp
Platform/System/Posix/CYBPosixThread.cpp
Platform/System/Posix/CYBPosixProcess.cpp
Platform/System/Posix/CYBPosixVirtualMemory.cpp
Platform/System/Posix/CYBPosixMutex.cpp


#Modules
Platform/Modules/CYBModule.cpp

#Posix
Platform/Modules/Posix/CYBPosixModule.cpp



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