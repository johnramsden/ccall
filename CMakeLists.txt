cmake_minimum_required(VERSION 3.8)
project(ccall)

set(CMAKE_C_STANDARD 11)

list(APPEND LINPHONE_LINK_LIBRARIES "")

message("\nSearching for linphone libraries")


find_library(LINPHONE_LIB REQUIRED NAMES linphone)

if(LINPHONE_LIB)
    message("linphone library found at: ${LINPHONE_LIB}")
    list(APPEND LINPHONE_LINK_LIBRARIES "${LINPHONE_LIB}")
else()
    message(FATAL_ERROR "linphone library not found")
endif()

set(SOURCE_FILES main.c)
add_executable(ccall ${SOURCE_FILES})

target_link_libraries(ccall ${LINPHONE_LINK_LIBRARIES})