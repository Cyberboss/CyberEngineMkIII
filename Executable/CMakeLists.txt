cmake_minimum_required (VERSION 3.5)

project(Executable)

add_executable(CyberEngineMkII CYBEntry.cpp)

target_link_libraries(CyberEngineMkII EngineBase)
