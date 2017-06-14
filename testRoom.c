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
    //char *badEmail3 = "@@";

    int id = 5, price = 4 * 15, num_ppl = 4, open_time = 12, close_time = 16, goodDifficulty = 8, badDifficulty = 12;
    //        P_e = 2, skill_level = 1, test = -2;

    //int outputID = 0, output_price = 0, output_num_of_people = 0, output_room_difficulty = 0;
    //double room_recommended_calculation = 0;
    Room room = NULL;
    //Room copy_of_room = NULL;
    //Room second_room = NULL;
    roomResult result;

    //createRoom tests
    room = createRoom(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty, &result);
    ASSERT_TEST(result == ROOMS_SUCCESS);

    //second_room = createRoom(goodEmail, (id + 10), price, num_ppl, open_time, close_time, goodDifficulty, &result);
    //ASSERT_TEST(result == ROOMS_SUCCESS);

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

    return true;
}

static bool testcopyRoom() {
    char *goodEmail = "asaf@gmail.com";

    int id = 5, price = 4 * 15, num_ppl = 4, open_time = 12, close_time = 16, goodDifficulty = 8;

    int outputID = 0, output_price = 0, output_num_of_people = 0, output_room_difficulty = 0;
    Room room = NULL;
    Room copy_of_room = NULL;
    roomResult result;

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

    removeRoom(room);
    removeRoom(copy_of_room);

    return true;
}

static bool testRoomCalculation() {
    char *goodEmail = "asaf@gmail.com";

    int id = 5, price = 4 * 15, num_ppl = 4, open_time = 12, close_time = 16, goodDifficulty = 8, P_e = 2,
            skill_level = 1;

    double room_recommended_calculation = 0;
    Room room = NULL;
    roomResult result;

    room = createRoom(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty, &result);
    ASSERT_TEST(result == ROOMS_SUCCESS);

    //tests room calculation
    room_recommended_calculation = getRoomRecommendedCalculation(room, P_e, skill_level);
    ASSERT_TEST(room_recommended_calculation == 53);

    removeRoom(room);

    return true;
}

static bool testcompareRoom() {
    char *goodEmail = "asaf@gmail.com";

    int id = 5, price = 4 * 15, num_ppl = 4, open_time = 12, close_time = 16, goodDifficulty = 8, test = -2;

    Room room = NULL;
    Room copy_of_room = NULL;
    Room second_room = NULL;
    roomResult result;

    room = createRoom(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty, &result);
    ASSERT_TEST(result == ROOMS_SUCCESS);

    second_room = createRoom(goodEmail, (id + 10), price, num_ppl, open_time, close_time, goodDifficulty, &result);
    ASSERT_TEST(result == ROOMS_SUCCESS);

    copy_of_room = copyRoom(room);

    //tests compare room
    test = compareRoom(room, copy_of_room);
    ASSERT_TEST(test == 0);
    test = compareRoom(room, second_room);
    ASSERT_TEST(test == -10);

    removeRoom(room);
    removeRoom(copy_of_room);
    removeRoom(second_room);

    return true;
}


int main (int argv, char** arc){

    RUN_TEST(testCreateRoom);
    RUN_TEST(testcopyRoom);
    RUN_TEST(testRoomCalculation);
    RUN_TEST(testcompareRoom);



    return 0;
}

