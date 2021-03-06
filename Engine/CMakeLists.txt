include_directories(${CYB_BASE_DIR}/Engine)

find_package(Threads REQUIRED)

if(APPLE)
set(CYB_PLATFORM_CODE_FILES Platform/System/OSX/CYBOSXProcess.cpp Platform/System/OSX/CYBOSXThread.cpp Platform/System/OSX/CYBOSXPath.cpp)
else()
set(CYB_PLATFORM_CODE_FILES Platform/System/Linux/CYBLinuxProcess.cpp Platform/System/Linux/CYBLinuxThread.cpp Platform/System/Linux/CYBLinuxPath.cpp)
endif()

add_library(EngineBase STATIC


#Platform

#System
Platform/System/CYBThread.cpp
Platform/System/CYBPath.cpp
Platform/System/CYBFile.cpp
Platform/System/CYBProcess.cpp
Platform/System/CYBVirtualMemory.cpp
Platform/System/CYBSemaphore.cpp
Platform/System/CYBConsole.cpp

${CYB_PLATFORM_CODE_FILES}

#Posix
Platform/System/Posix/CYBPosixSyscalls.cpp
Platform/System/Posix/CYBPosixThread.cpp
Platform/System/Posix/CYBPosixProcess.cpp
Platform/System/Posix/CYBPosixVirtualMemory.cpp
Platform/System/Posix/CYBPosixMutex.cpp
Platform/System/Posix/CYBPosixSemaphore.cpp
Platform/System/Posix/CYBPosixPath.cpp
Platform/System/Posix/CYBPosixFile.cpp
Platform/System/Posix/CYBPosixConsole.cpp


#Modules
Platform/Modules/CYBModule.cpp

#Posix
Platform/Modules/Posix/CYBPosixModule.cpp



#Engine

#Memory
Engine/Memory/CYBBlock.cpp
Engine/Memory/CYBLargeBlock.cpp
Engine/Memory/CYBHeap.cpp

#Helpers
Engine/Helpers/CYBCommandLine.cpp

#Core
Engine/Core/CYBEngineParameters.cpp
Engine/Core/CYBLogger.cpp
Engine/Core/CYBException.cpp
Engine/Core/CYBInterop.cpp
Engine/Core/CYBCore.cpp
)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
	set_property(TARGET EngineBase PROPERTY COMPILE_FLAGS "-fprofile-arcs -ftest-coverage -pedantic -fno-inline")
	target_link_libraries(EngineBase dl -fprofile-arcs -ftest-coverage -fno-inline)
ELSE()
	set_property(TARGET EngineBase PROPERTY COMPILE_FLAGS "-pedantic")
	target_link_libraries(EngineBase dl)
ENDIF()

set_property (SOURCE "CYB.cpp" PROPERTY COTIRE_DEPENDENCY "TRUE")
set_target_properties(EngineBase PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "CYB.hpp")
cotire(EngineBase)