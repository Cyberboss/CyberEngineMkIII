cmake_minimum_required (VERSION 3.4.3)

set(CYB_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

include(${CYB_BASE_DIR}/BuildScripts/CMake/cotire.cmake)
include(${CYB_BASE_DIR}/BuildScripts/CMake/add_catch_tests.cmake)

Project(CyberEngineMarkIII)

enable_testing()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CYB_BASE_DIR}/Output/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CYB_BASE_DIR}/Output/bin")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "\$ORIGIN")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2 -Wall -Werror -Wdocumentation -fmodules -fvisibility=hidden -D__USE_XOPEN2K8 -D_FILE_OFFSET_BITS=64")
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")

#support libs here

add_subdirectory(Engine)
add_subdirectory(Executable)
add_subdirectory(Tests)
