IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
set(CYB_EXE_NAME "CyberEngineMkIIID")
ELSE()
set(CYB_EXE_NAME "CyberEngineMkIII")
ENDIF()

add_executable(${CYB_EXE_NAME} 
CYBEntry.cpp)

target_link_libraries(${CYB_EXE_NAME} EngineBase)
