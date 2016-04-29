cmake_minimum_required (VERSION 3.4.3)

IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
set(CYB_EXE_NAME "CyberEngineMkIIID")
ELSE()
set(CYB_EXE_NAME "CyberEngineMkIII")
ENDIF()

add_executable(${CYB_EXE_NAME} CYBEntry.cpp)

set_property(TARGET ${CYB_EXE_NAME} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${CYB_EXE_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(${CYB_EXE_NAME} EngineBase)
