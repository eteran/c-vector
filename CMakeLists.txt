cmake_minimum_required(VERSION 3.0.0)
project(c-vector VERSION 0.1.0)

add_executable(c-vector-example
	example.c
)

enable_testing()
set(TEST_EXE "test-c-vector")

add_executable(${TEST_EXE}
	EXCLUDE_FROM_ALL
	test.c
)

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
