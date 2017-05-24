cmake_minimum_required(VERSION 3.7)
project(HW03_Part2)

set(CMAKE_C_STANDARD 99)
SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic-errors -Werror -DNDEBUG")
SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )


set(SOURCE_FILES main.c list.h mtm_ex3.h set.h libmtm/win64/libmtm.a)
add_executable(HW03_Part2 ${SOURCE_FILES})