cmake_minimum_required(VERSION 3.0.0)
project(c-vector VERSION 0.1.0)

add_library(c-vector cvector.c)
add_executable (c-vector-example cvector.c example.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
