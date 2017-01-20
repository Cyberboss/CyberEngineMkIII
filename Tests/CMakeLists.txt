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
Platform/System/Semaphore.cpp
Platform/System/Console.cpp

Engine/Memory/Blocks.cpp
Engine/Memory/Heap.cpp

Engine/Core/Core.cpp
Engine/Core/Logger.cpp

API/Types/StaticAndCStyleString.cpp
API/Types/DynamicString.cpp
API/Types/UTF8String.cpp
API/Types/UTF16String.cpp
API/Types/STLPrototypes.cpp

API/Interop/Allocator.cpp
API/Interop/STLAllocator.cpp

)

target_link_libraries(CyberEngineMkIIITester EngineBase)

set_property (SOURCE "TestHeader.cpp" PROPERTY COTIRE_DEPENDENCY "TRUE")
set_target_properties(CyberEngineMkIIITester PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "TestHeader.hpp")
cotire(CyberEngineMkIIITester)

add_catch_tests(CyberEngineMkIIITester)
file(MAKE_DIRECTORY ${CYB_BASE_DIR}/Output/Resources)