cmake_minimum_required(VERSION 3.8)
project(ccall)

set(CMAKE_C_STANDARD 11)

if(NOT FOO_LIBRARY)
    add_subdirectory(external/foo)
endif()

set(SOURCE_FILES main.c)
add_executable(ccall ${SOURCE_FILES})