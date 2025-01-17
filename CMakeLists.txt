cmake_minimum_required(VERSION 3.20)

project(log
        VERSION 1.0.0
        LANGUAGES CXX)

set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART_REQUIRED ON)
set(CMAKE_LOG_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/liblog)

add_library(log SHARED ${CMAKE_LOG_LIBRARY_DIR}/Log.cpp)

add_executable(main src/main.cpp)

target_link_libraries(main PUBLIC log)

target_include_directories(main PUBLIC ${CMAKE_LOG_LIBRARY_DIR})

add_custom_target(
    TARGET clean         
    COMMAND rm -rf ${CMAKE_BINARY_DIR}/*.*
    COMMENT "Clean the build directory"
  )
#set_target_properties(clean PROPERTIES EXCLUDE_FROM_ALL TRUE)
