cmake_minimum_required(VERSION 3.20)
project(day10_project CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/z3/4.15.4/lib/cmake/z3")

find_package(z3 REQUIRED)

add_executable(day10 day10/day10.cc)

if(NOT TARGET z3::libz3)
    add_library(z3::libz3 ALIAS libz3)
endif()

target_link_libraries(day10 PRIVATE z3::libz3)
target_include_directories(day10 PRIVATE ${Z3_INCLUDE_DIR} ${Z3_CPP_INCLUDE_DIR})