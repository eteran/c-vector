cmake_minimum_required(VERSION 3.0.0)
project(c-vector VERSION 0.1.0)
add_compile_options(-Wall -Wextra)
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

enable_testing()
set(TEST_EXE "test-c-vector")

add_executable(${TEST_EXE}
	EXCLUDE_FROM_ALL
	test.c
)

set_target_properties(c-vector-example PROPERTIES C_STANDARD 99)
set_target_properties(${TEST_EXE} PROPERTIES C_STANDARD 99)

# 'make test' will not automatically rebuild the executable if the source
# has been changed; 'make' or 'make clean && make' need to be run first;
# but there is a workaround
# https://stackoverflow.com/questions/733475/cmake-ctest-make-test-doesnt-build-tests
# https://gitlab.kitware.com/cmake/community/-/wikis/doc/tutorials/EmulateMakeCheck
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
add_test(NAME test COMMAND ${TEST_EXE})
add_dependencies(check ${TEST_EXE})

find_program(VALGRIND "valgrind")
if(VALGRIND)
	add_custom_target(memcheck
	COMMAND "${VALGRIND}"
		--tool=memcheck
		--leak-check=full
		--show-reachable=yes
		--error-exitcode=1
		$<TARGET_FILE:${TEST_EXE}>
	)
endif()
