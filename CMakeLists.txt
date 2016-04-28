cmake_minimum_required (VERSION 3.4.3)

set(CYB_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CYB_BASE_DIR}/Output/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CYB_BASE_DIR}/Output/bin")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "\$ORIGIN")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2 -Wall -Werror")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -O3")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -O3")

#support libs here

include(${CYB_BASE_DIR}/BuildScripts/CMake/PrecompiledHeader.cmake)

add_subdirectory(Engine)
add_subdirectory(Executable)
add_subdirectory(Tests)