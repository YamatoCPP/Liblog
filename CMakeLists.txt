cmake_minimum_required(VERSION 3.28)
project(logger LANGUAGES CXX)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER g++)

include_directories(include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

add_executable(logexc src/main.cpp)
add_library(log SHARED include/liblog/log.cpp)

option(BUILD_TEST "Build testing" OFF)
if(BUILD_TEST)
    add_executable(tests tests/log_test.cpp)
    target_link_libraries(tests PRIVATE log)
endif()

target_compile_options(logexc PRIVATE -O3 -march=native
    -Wall -Werror -Wextra -Wpedantic)
    
target_link_libraries(logexc PRIVATE log)
