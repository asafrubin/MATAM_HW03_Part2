#include "test_utilities.h"
#include "mtm_ex3.h"
#include "Rooms.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


static bool testCreateRoom() {
    char *goodEmail = "asaf@gmail.com";
    char *badEmail1 = "nate@@gmail.com";
    char *badEmail2 = "BadEmail";
    char *badEmail3 = "@@";

    int id = 5, price = 4*15, num_ppl = 4, open_time = 12, close_time = 16, goodDifficulty = 8, badDifficulty = 12 ,
        P_e = 2, skill_level = 1 , test = -2;

    int outputID = 0, output_price = 0, output_num_of_people = 0, output_room_difficulty = 0;
    double room_recommended_calculation = 0;
    Room room = NULL;
    Room copy_of_room = NULL;
    Room second_room = NULL;
    roomResult result;

    //createRoom tests
    room = createRoom(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty, &result);
    ASSERT_TEST(result == ROOMS_SUCCESS);

    second_room = createRoom(goodEmail, (id + 10), price, num_ppl, open_time, close_time, goodDifficulty, &result);
    ASSERT_TEST(result == ROOMS_SUCCESS);

    //removeRoom(room);
    room = createRoom(badEmail1, id, price, num_ppl, open_time, close_time, goodDifficulty, &result);
    ASSERT_TEST(result == ROOMS_INVALID_PARAMETER);
    removeRoom(room);
    room = createRoom(goodEmail, id, price, num_ppl, open_time, close_time, badDifficulty, &result);
    ASSERT_TEST(result == ROOMS_INVALID_PARAMETER);
    removeRoom(room);
    room = createRoom(badEmail2, id, price, num_ppl, open_time, close_time, goodDifficulty, &result);
    ASSERT_TEST(result == ROOMS_INVALID_PARAMETER);
    removeRoom(room);


    //copyRoom tests - checks that all fields match
    room = createRoom(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty, &result);
    ASSERT_TEST(result == ROOMS_SUCCESS);

    copy_of_room = copyRoom(room);

    //uses get functions to compare all fields
    outputID = getRoomId(copy_of_room);
    ASSERT_TEST(outputID == id);

    output_price = getRoomPrice(copy_of_room);
    ASSERT_TEST(output_price == price);

    output_num_of_people = getRoomNumOfPpl(copy_of_room);
    ASSERT_TEST(output_num_of_people == num_ppl);

    output_room_difficulty = getRoomDifficulty(copy_of_room);
    ASSERT_TEST(output_room_difficulty == goodDifficulty);

    //tests room calculation
    room_recommended_calculation = getRoomRecommendedCalculation(room, P_e, skill_level);
    ASSERT_TEST(room_recommended_calculation == 53);
k
    //tests compare room
    test = compareRoom(room, copy_of_room);
    ASSERT_TEST(test == 0);
    test = compareRoom(room, second_room);
    ASSERT_TEST(test == -10);

    //tests check room parameters
    result = checkRoomParameters(badEmail3, id, price, num_ppl, open_time, close_time, goodDifficulty);
    ASSERT_TEST(result == ROOMS_INVALID_PARAMETER);

    result = checkRoomParameters(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty);
    ASSERT_TEST(result == ROOMS_SUCCESS);

    removeRoom(room);
    removeRoom(copy_of_room);
    removeRoom(second_room);

    //void* setCopyRoom(void* roomToCopy);

    //void setRemoveRoom(void* roomToRemove);

    //int setCompareRoom(void* firstRoom , void* secondRoom);

    return true;
}

int main (int argv, char** arc){

    RUN_TEST(testCreateRoom);

    return 0;
}

