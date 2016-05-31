cmake_minimum_required (VERSION 3.4.3)

include_directories(${CYB_BASE_DIR}/Tests)

add_executable(CyberEngineMkIIITester

PosixMain.cpp
FakeCore.cpp

Platform/VirtualMemory.cpp
Platform/Modules.cpp
Platform/Threads.cpp

Engine/Memory/Blocks.cpp
Engine/Memory/Heap.cpp

)

set_property(TARGET CyberEngineMkIIITester PROPERTY CXX_STANDARD 14)
set_property(TARGET CyberEngineMkIIITester PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(CyberEngineMkIIITester EngineBase)

set_property (SOURCE "TestHeader.cpp" PROPERTY COTIRE_DEPENDENCY "TRUE")
set_target_properties(CyberEngineMkIIITester PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "TestHeader.hpp")
cotire(CyberEngineMkIIITester)

add_catch_tests(CyberEngineMkIIITester)