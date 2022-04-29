cmake_minimum_required(VERSION 3.0.0)
project(c-vector VERSION 0.1.0)
enable_testing()

# ------ cvector library ------
add_library(${CMAKE_PROJECT_NAME} INTERFACE)

target_sources(c-vector INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/cvector.h
	${CMAKE_CURRENT_SOURCE_DIR}/cvector_utils.h
)

# ------------------------------

# ------ simple example ------
add_executable(c-vector-example
	example.c
)

target_link_libraries(c-vector-example
PUBLIC
	${CMAKE_PROJECT_NAME}
)

set_target_properties(c-vector-example PROPERTIES C_STANDARD 99)
target_compile_options(c-vector-example PUBLIC -Wall -Wextra)

# ----------------------------

# ------ test executable used for memory checks ------
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
add_test(NAME test COMMAND $<TARGET_FILE:${TEST_EXE}>)
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

set_target_properties(${TEST_EXE} PROPERTIES C_STANDARD 99)
target_compile_options(${TEST_EXE} PUBLIC -Wall -Wextra)

# ----------------------------------------------------

# ------ unit tests ------

set(UNIT_TEST_EXE "unit-tests")
set(UNIT_TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/unit-tests.c)
add_executable(${UNIT_TEST_EXE} ${UNIT_TEST_SOURCES})

add_test(NAME ${UNIT_TEST_EXE} COMMAND $<TARGET_FILE:${UNIT_TEST_EXE}>)

set_target_properties(${UNIT_TEST_EXE} PROPERTIES C_STANDARD 99)
target_compile_options(${UNIT_TEST_EXE} PUBLIC -Wall -Wextra)

# ------------------------
