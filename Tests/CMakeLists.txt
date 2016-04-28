cmake_minimum_required (VERSION 3.4.3)


project(Tests)

add_executable(CyberEngineMkIIITester
Main.cpp
VirtualMemory.cpp
)

add_definitions(-D__USE_XOPEN2K8)

set_property(TARGET CyberEngineMkIIITester PROPERTY CXX_STANDARD 14)
set_property(TARGET CyberEngineMkIIITester PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(CyberEngineMkIIITester EngineBase)

add_custom_command(TARGET CyberEngineMkIIITester POST_BUILD COMMAND CyberEngineMkIIITester)