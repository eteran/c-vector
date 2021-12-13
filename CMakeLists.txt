cmake_minimum_required(VERSION 3.0.0)
project(c-vector VERSION 0.1.0)

add_library(${CMAKE_PROJECT_NAME} INTERFACE)

target_sources(c-vector INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/cvector.h
)

add_executable(c-vector-example
	example.c
)

target_link_libraries(c-vector-example
PUBLIC
	${CMAKE_PROJECT_NAME}
)

# automatically invokes the 'enable_testing()' command (except when the
# BUILD_TESTING option is turned off).
include(CTest)

set(TEST_EXE "test-c-vector")

# 'make test' will not automatically rebuild the executable if the source
# has been changed; 'make' or 'make clean && make' need to be run first.
add_executable(${TEST_EXE}
	test.c
)

add_test(NAME test COMMAND ${TEST_EXE})

