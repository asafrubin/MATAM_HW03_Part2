cmake_minimum_required(VERSION 3.7)
project(HW03_Part2)

set(CMAKE_C_STANDARD 99)
SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic-errors -Werror -DNDEBUG")
SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

#Whole Program
#set(SOURCE_FILES  main.c list.h mtm_ex3.h set.h libmtm/win64/libmtm.a EscapeTechnion.c EscapeTechnion.h Rooms.c Rooms.h Orders.c Orders.h Escapers.c Escapers.h Companies.c Companies.h parser.c)

#test Rooms
#set(SOURCE_FILES test_utilities.h testRoom.c  mtm_ex3.h Rooms.c Rooms.h)

#test Orders
#set(SOURCE_FILES test_utilities.h testOrders.c  mtm_ex3.h Orders.c Orders.h)

#test Escapers
set(SOURCE_FILES test_utilities.h testEscapers.c  mtm_ex3.h Escapers.c Escapers.h testEscapers.c)

add_executable(HW03_Part2 ${SOURCE_FILES})