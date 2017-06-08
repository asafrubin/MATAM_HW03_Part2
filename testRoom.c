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

    int id = 5, price = 4*15, num_ppl = 4, open_time = 12, close_time = 16, goodDifficulty = 8, badDifficulty = 12;
    Room room = NULL;
    roomResult result;

    room = createRoom(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty, &result);
    ASSERT_TEST(result == ROOMS_SUCCESS);
    removeRoom(room);
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

int main (int argv, char** arc){

    RUN_TEST(testCreateRoom);

    return 0;
}

