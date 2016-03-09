cmake_minimum_required (VERSION 3.4.3)

project(Engine)

include_directories(${CYB_BASE_DIR}/Engine)

add_library(EngineBase STATIC
CYBSource.cpp)

add_precompiled_header(EngineBase CYB.hpp)

set_property(TARGET EngineBase PROPERTY CXX_STANDARD 14)
set_property(TARGET EngineBase PROPERTY CXX_STANDARD_REQUIRED ON)