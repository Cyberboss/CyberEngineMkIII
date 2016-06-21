cmake_minimum_required (VERSION 3.4.3)

include_directories(${CYB_BASE_DIR}/Tests)

add_executable(CyberEngineMkIIITester

PosixMain.cpp
Utils/Fakes.cpp

Platform/Modules.cpp

Platform/System/VirtualMemory.cpp
Platform/System/Threads.cpp
Platform/System/Mutex.cpp
Platform/System/Process.cpp

Engine/Memory/Blocks.cpp
Engine/Memory/Heap.cpp

Engine/Core/Exception.cpp
Engine/Core/Core.cpp

API/String/StaticAndCStyleString.cpp
API/String/DynamicString.cpp

)

set_property(TARGET CyberEngineMkIIITester PROPERTY CXX_STANDARD 14)
set_property(TARGET CyberEngineMkIIITester PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(CyberEngineMkIIITester EngineBase)

set_property (SOURCE "TestHeader.cpp" PROPERTY COTIRE_DEPENDENCY "TRUE")
set_target_properties(CyberEngineMkIIITester PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "TestHeader.hpp")
cotire(CyberEngineMkIIITester)

add_catch_tests(CyberEngineMkIIITester)

find_package(PythonInterp 3.2 REQUIRED)

IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
add_test(NAME InitLcovData COMMAND lcov --directory . --base-directory . --gcov-tool Tests/llvm-gcov.sh --no-external --capture --initial -o lcovbase.dat WORKING_DIRECTORY ${CYB_BASE_DIR})
add_test(NAME GenerateLcovData COMMAND lcov --directory . --base-directory . --gcov-tool Tests/llvm-gcov.sh --no-external --capture -o lcovrun.dat WORKING_DIRECTORY ${CYB_BASE_DIR})
add_test(NAME AddInitialData COMMAND lcov -a lcovbase.dat -a lcovrun.dat -o lcovwip.dat WORKING_DIRECTORY ${CYB_BASE_DIR})
add_test(NAME RemovingExcludedLcovData COMMAND lcov -r lcovwip.dat \*Assert.inl \*CYBSyscalls.inl \*.hpp -o lcov.dat WORKING_DIRECTORY ${CYB_BASE_DIR})
add_test(NAME GenerateLcovHtml COMMAND genhtml lcov.dat -o HTMLCodeCoverage WORKING_DIRECTORY ${CYB_BASE_DIR})
add_test(NAME DownloadCoberturaConverter COMMAND curl -O https://raw.githubusercontent.com/eriwen/lcov-to-cobertura-xml/master/lcov_cobertura/lcov_cobertura.py WORKING_DIRECTORY ${CYB_BASE_DIR})
add_test(NAME GenerateCoberturaXML COMMAND ${PYTHON_EXECUTABLE} lcov_cobertura.py lcov.dat -o Code.coveragexml WORKING_DIRECTORY ${CYB_BASE_DIR})
add_test(NAME RemovePreviousCodeCoverageHTML COMMAND sudo rm -rf /srv/http WORKING_DIRECTORY ${CYB_BASE_DIR})
add_test(NAME PublishHTMLCodeCoverageResultsToLocalWebServer COMMAND sudo cp -r HTMLCodeCoverage/ /srv/http WORKING_DIRECTORY ${CYB_BASE_DIR})
ENDIF()