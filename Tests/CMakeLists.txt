cmake_minimum_required (VERSION 3.4.3)

add_executable(CyberEngineMkIIITester
PosixMain.cpp
VirtualMemory.cpp
Modules.cpp
Threads.cpp
)

add_definitions(-D__USE_XOPEN2K8)

set_property(TARGET CyberEngineMkIIITester PROPERTY CXX_STANDARD 14)
set_property(TARGET CyberEngineMkIIITester PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(CyberEngineMkIIITester EngineBase)

set_property (SOURCE "TestHeader.cpp" PROPERTY COTIRE_DEPENDENCY "TRUE")
set_target_properties(CyberEngineMkIIITester PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "TestHeader.hpp")
cotire(CyberEngineMkIIITester)