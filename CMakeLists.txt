cmake_minimum_required(VERSION 3.3)
project(umps_uarm_hello_world VERSION 0.1.0 LANGUAGES C ASM)

set(SRC ${PROJECT_SOURCE_DIR})
set(INC ${PROJECT_SOURCE_DIR}/include/)

# Set C standard and compilation flags
set(CMAKE_C_STANDARD 90)
set(CFLAGS_LANG -ffreestanding -ansi -Wall)

# PROJECT_SOURCE_DIR e PROJECT_BINARY_DIR are CMake variables
include_directories(${INC})
include_directories(${INC})
add_compile_options(${CFLAGS_LANG})

add_library(termprint ${SRC}/termprint.c)

# Conditional inclusion of targets upon the detected architecture
if (CMAKE_SYSTEM_PROCESSOR STREQUAL mips)
	add_definitions(-DTARGET_UMPS)
	include(umps.cmake)
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
	add_definitions(-DTARGET_UARM)
	include(uarm.cmake)
else()
	message(FATAL_ERROR "You need to specify a toolchain file with either -D
	CMAKE_TOOLCHAIN_FILE=toolchains/umps.cmake or -D
	CMAKE_TOOLCHAIN_FILE=toolchains/uarm.cmake")
endif()
