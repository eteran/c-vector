cmake_minimum_required(VERSION 3.7.0)
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

set_target_properties(c-vector-example PROPERTIES C_STANDARD 90)
target_compile_options(c-vector-example PUBLIC -Wall -Werror -Wextra)

# ----------------------------

# ------ test executable used for memory checks ------
add_executable(test-c-vector
	EXCLUDE_FROM_ALL
	${CMAKE_CURRENT_SOURCE_DIR}/test.c
)

add_test(NAME test-c-vector COMMAND test-c-vector)
set_target_properties(test-c-vector PROPERTIES C_STANDARD 90)
target_compile_options(test-c-vector PUBLIC -Wall -Werror -Wextra)

add_test(test_build
  "${CMAKE_COMMAND}"
  --build "${CMAKE_BINARY_DIR}"
  --config "$<CONFIG>"
  --target test-c-vector
)
set_tests_properties(test_build PROPERTIES FIXTURES_SETUP    test_fixture)
set_tests_properties(test-c-vector PROPERTIES FIXTURES_REQUIRED test_fixture)

find_program(VALGRIND "valgrind")
if(VALGRIND)
	add_custom_target(memcheck
	COMMAND "${VALGRIND}"
		--tool=memcheck
		--leak-check=full
		--show-reachable=yes
		--error-exitcode=1
		$<TARGET_FILE:test-c-vector>
	)
endif()

# ----------------------------------------------------

# ------ unit tests ------

add_executable(unit-tests
	EXCLUDE_FROM_ALL
	${CMAKE_CURRENT_SOURCE_DIR}/unit-tests.c
)

add_test(NAME unit-tests COMMAND $<TARGET_FILE:unit-tests>)
set_target_properties(unit-tests PROPERTIES C_STANDARD 90)
target_compile_options(unit-tests PUBLIC -Wall -Werror -Wextra)

add_test(unit_test_build
  "${CMAKE_COMMAND}"
  --build "${CMAKE_BINARY_DIR}"
  --config "$<CONFIG>"
  --target unit-tests
)
set_tests_properties(unit_test_build PROPERTIES FIXTURES_SETUP test_fixture)
set_tests_properties(unit-tests PROPERTIES FIXTURES_REQUIRED test_fixture)

# ------------------------
