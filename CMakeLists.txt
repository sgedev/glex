#
# glex main cmake file.
#

cmake_minimum_required(VERSION 3.19)

project(glex VERSION 0.1.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/lib")

set(CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/cmake")

file(GLOB_RECURSE GLEX_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.in
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    )

set(GLEX_DEBUG ON CACHE BOOL "Enable debug functions.")

set(GLEX_SHARED ON CACHE BOOL "Build as shared library.")
if(GLEX_SHARED)
	add_library(glex SHARED ${GLEX_SOURCES})
    target_compile_definitions(glex PRIVATE GLEX)
else()
	add_library(glex STATIC ${GLEX_SOURCES})
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GLEX_SOURCES})

target_include_directories(glex
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

configure_file(
	include/GL/glex.h.in
	include/GL/glex.h
	)

add_subdirectory(tools tools)
add_subdirectory(test test)
add_subdirectory(docs docs)
