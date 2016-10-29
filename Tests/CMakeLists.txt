include_directories(${CYB_BASE_DIR}/Tests)

add_executable(CyberEngineMkIIITester

PosixMain.cpp
Utils/Fakes.cpp
Utils/HCF.cpp

Platform/Modules.cpp

Platform/System/VirtualMemory.cpp
Platform/System/Threads.cpp
Platform/System/Mutex.cpp
Platform/System/File.cpp
Platform/System/Path.cpp
Platform/System/Process.cpp

Engine/Memory/Blocks.cpp
Engine/Memory/Heap.cpp

Engine/Core/Exception.cpp
Engine/Core/Core.cpp

API/String/StaticAndCStyleString.cpp
API/String/DynamicString.cpp
API/String/UTF8String.cpp
API/String/UTF16String.cpp

API/Interop/Allocator.cpp
API/Interop/STLAllocator.cpp

)

set_property(TARGET CyberEngineMkIIITester PROPERTY CXX_STANDARD 17)
set_property(TARGET CyberEngineMkIIITester PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(CyberEngineMkIIITester EngineBase)

set_property (SOURCE "TestHeader.cpp" PROPERTY COTIRE_DEPENDENCY "TRUE")
set_target_properties(CyberEngineMkIIITester PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "TestHeader.hpp")
cotire(CyberEngineMkIIITester)

add_catch_tests(CyberEngineMkIIITester)
file(MAKE_DIRECTORY ${CYB_BASE_DIR}/Output/Resources)