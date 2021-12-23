
if(GLEX_SHARED)
	message(WARNING "glex-test only available on static glex.")
	return()
endif()

set(GLEX_TEST ON CACHE BOOL "Build test.")
if(NOT GLEX_TEST)
	return()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

link_libraries(glex)

include(FindAssimp)
if(ASSIMP_FOUND)
	include_directories(${ASSIMP_INCLUDE_DIR})
	link_libraries(${ASSIMP_LIBRARIES})
else()
	message(FATAL_ERROR "Assimp not found.")
endif()

include(FindGLFW)
if(GLFW_FOUND)
	include_directories(${GLFW_INCLUDE_DIRS})
	link_libraries(${GLFW_LIBRARIES})
else()
	message(FATAL_ERROR "GLFW not found.")
endif()

file(GLOB_RECURSE GLEX_TEST_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.in
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

add_executable(glex-test ${GLEX_TEST_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GLEX_TEST_SOURCES})
