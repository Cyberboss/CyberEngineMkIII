cmake_minimum_required (VERSION 3.2.2)

Project(CyberEngineMarkIII)

set(CYB_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

include(${CYB_BASE_DIR}/BuildScripts/CMake/cotire.cmake)
include(${CYB_BASE_DIR}/BuildScripts/CMake/add_catch_tests.cmake)

enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CYB_BASE_DIR}/Output/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CYB_BASE_DIR}/Output/bin")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "\$ORIGIN")

if(APPLE)
	set(OSX_CXX_FLAGS "-Wno-keyword-macro")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2 -Wall -Werror -Wdocumentation -Wno-c++11-extra-semi ${OSX_CXX_FLAGS} -fvisibility=hidden -D__USE_XOPEN2K8 -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_DARWIN_FEATURE_64_BIT_INODE")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

option(GOLD_PLUGIN "Use the llvm gold plugin for link time code generation. RECOMMENDED" ON)
if(GOLD_PLUGIN)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
endif()

#support libs here

add_subdirectory(Engine)
add_subdirectory(Executable)
add_subdirectory(Tests)
