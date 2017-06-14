cmake_minimum_required(VERSION 3.7)
project(HW03_Part2)

set(CMAKE_C_STANDARD 99)
SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic-errors -Werror -DNDEBUG")
SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

#Whole Program
set(SOURCE_FILES parser.c list.h mtm_ex3.h set.h EscapeTechnion.c EscapeTechnion.h Rooms.c Rooms.h Orders.c Orders.h Escapers.c Escapers.h Companies.c Companies.h)

#test Rooms
#set(SOURCE_FILES test_utilities.h testRoom.c mtm_ex3.h Rooms.c Rooms.h)

#test Orders
#set(SOURCE_FILES test_utilities.h testOrders.c  mtm_ex3.h Orders.c Orders.h)

#test EscapeTechnion
#set(SOURCE_FILES test_utilities.h set.h list.h mtm_ex3.h Orders.c Orders.h Rooms.c Rooms.h Companies.c Companies.h Escapers.c Escapers.h EscapeTechnion.c EscapeTechnion.h testEscapeTechnion.c)

#test Companies
#set(SOURCE_FILES test_utilities.h  mtm_ex3.h Companies.h Companies.c testCompanies.c Rooms.c Rooms.h)

#test Escapers
#set(SOURCE_FILES test_utilities.h testEscapers.c  mtm_ex3.h Escapers.c Escapers.h set.h list.h)


link_directories(libmtm/win32)

add_executable(HW03_Part2 ${SOURCE_FILES})

target_link_libraries(HW03_Part2 libmtm.a)
