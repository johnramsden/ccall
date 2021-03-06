cmake_minimum_required(VERSION 3.8)
project(ccall)

set(CCALL_MAJOR_VERSION 0)
set(CCALL_MINOR_VERSION 1)
set(CCALL_PATCH_VERSION 1)
set(CCALL_VERSION
  ${CCALL_MAJOR_VERSION}.${CCALL_MINOR_VERSION}.${CCALL_PATCH_VERSION})

set(CMAKE_C_STANDARD 11)

## Find required libraries ##

list(APPEND CCALL_LINK_LIBRARIES "")

message("\nSearching for libraries")

find_library(LINPHONE_LIB REQUIRED NAMES linphone)
find_library(NCURSES_LIB REQUIRED NAMES ncurses)
find_library(LIBMEDIASTREAMER_LIB REQUIRED NAMES libmediastreamer_base mediastreamer_base)

if(NCURSES_LIB)
    message("ncurses library found at: ${NCURSES_LIB}")
    list(APPEND CCALL_LINK_LIBRARIES "${NCURSES_LIB}")
else()
    message(FATAL_ERROR "ncurses library not found")
endif()

if(LINPHONE_LIB)
    message("linphone library found at: ${LINPHONE_LIB}")
    list(APPEND CCALL_LINK_LIBRARIES "${LINPHONE_LIB}")
else()
    message(FATAL_ERROR "linphone library not found")
endif()

if(LIBMEDIASTREAMER_LIB)
    message("libmediastreamer library found at: ${LIBMEDIASTREAMER_LIB}")
    list(APPEND CCALL_LINK_LIBRARIES "${LIBMEDIASTREAMER_LIB}")
else()
    message(FATAL_ERROR "libmediastreamer library not found")
endif()

set(SOURCE_FILES src/ccall.c include/ccall.h)

## Link and Compile ##

add_executable(ccall ${SOURCE_FILES})

target_link_libraries(ccall ${CCALL_LINK_LIBRARIES})

## Install ##

install( TARGETS ccall RUNTIME DESTINATION bin )

# Compile example: make DESTDIR=/usr install
