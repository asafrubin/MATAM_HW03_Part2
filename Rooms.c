#include "Rooms.h"
#include "set.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

roomResult static checkEmail(char *name);

struct SRooms{
    char *email;
    int id;
    int price;
    int num_of_ppl;
    int time_start;
    int time_finish;
    int difficulty;
};

/**
 * Creates an element of room type
 * @param email : company room email
 * @param id : unique id of email
 * @param price : price for room (multipliers of 4)
 * @param num_ppl : number of people recommend
 * @param time_start : room openning time
 * @param time_finish : room closing time
 * @param difficulty : room difficulty
 * @param result : roomResult (ENUM)
 * @return : struct of newly created room
 */
Room createRoom(char *email , int id , int price , int num_ppl , int time_start , int time_finish , int difficulty
                      , roomResult *result)
{

    Room newRoom = NULL;

    if(email == NULL){
        *result = ROOMS_NULL_PARAMETER;
        return NULL;
    }

    if(checkRoomParameters(email, id, price, num_ppl, time_start, time_finish, difficulty) == ROOMS_INVALID_PARAMETER){
        *result =  ROOMS_INVALID_PARAMETER;
        return NULL;
    }

    newRoom = malloc(sizeof(*newRoom));
    if (newRoom == NULL){
        *result = ROOMS_OUT_OF_MEMORY;
        return NULL;
    }
    newRoom->email = malloc(strlen(email) + 1);
    if (newRoom->email == NULL){
        free(newRoom);
        *result = ROOMS_OUT_OF_MEMORY;
        return NULL;
    }
    strcpy(newRoom->email , email);
    newRoom->id = id;
    newRoom->price = price;
    newRoom->num_of_ppl = num_ppl;
    newRoom->time_start = time_start;
    newRoom->time_finish = time_finish;
    newRoom->difficulty = difficulty;

    *result = ROOMS_SUCCESS;
    return newRoom;
}

/**
 * Checks that given parameters are acceptable to create a room
 * @param email : company email
 * @param id : room id
 * @param price : room price
 * @param num_ppl : number of people
 * @param time_start : room openning time
 * @param time_finish : room close time
 * @param difficulty : room difficulty
 * @return : roomResult (ENUM)
 */
roomResult checkRoomParameters(char *email, int id, int price, int num_ppl,
                               int time_start, int time_finish, int difficulty)
{
    assert(email != NULL);

    if(time_start < MIN_OPEN_TIME || time_start > MAX_OPEN_TIME || time_finish < MIN_OPEN_TIME
       || time_finish > MAX_OPEN_TIME || time_start >= time_finish){
        return ROOMS_INVALID_PARAMETER;

    }
    if(difficulty < MIN_DIFFICULTY || difficulty > MAX_DIFFICULTY || num_ppl <= 0 || id <= 0){
        return ROOMS_INVALID_PARAMETER;
    }

    if( (price % 4) != 0 ){
        return ROOMS_INVALID_PARAMETER;
    }

    if( checkEmail(email) != ROOMS_SUCCESS ){
        return ROOMS_INVALID_PARAMETER;
    }

    return ROOMS_SUCCESS;
}

/**
 *Checks if room is open during requested hour
 * @param room : room to check
 * @param req_hour : hour requested
 * @return : roomResult (ENUM)
 */
roomResult roomCheckIfOpen(Room room, int req_hour)
{
    assert(room != NULL);
    if(room->time_start <= req_hour && req_hour < room->time_finish){
        return ROOMS_SUCCESS;
    }

    return ROOMS_ROOM_NOT_AVAILABLE;
}

/**
 * Creates a copy of a given room
 * @param roomToCopy : room to copy
 * @return : returns a room struct which is identical to given room
 */
Room copyRoom(Room roomToCopy)
{
    Room copyOfRoom = NULL;
    roomResult result;

    copyOfRoom = createRoom(roomToCopy->email , roomToCopy->id , roomToCopy->price, roomToCopy->num_of_ppl,
                            roomToCopy->time_start, roomToCopy->time_finish , roomToCopy->difficulty , &result);

    return copyOfRoom;
}

/**
 * Removes a given room
 * @param room : room to remove
 */
void removeRoom(Room room)
{
    if(room){
        assert(room->email != NULL);
        free(room->email);
        free(room);
        }

}

/**
 * Compares between to rooms by their id
 * @param firstRoom
 * @param secondRoom
 * @return '0' if identical, else other int
 */
int compareRoom(Room firstRoom, Room secondRoom)
{
    return ((firstRoom->id) - (secondRoom->id));
}

/**
 * Checks if email is valid according to specific rules
 * @param name : email
 * @return roomResult (ENUM)
 */
roomResult static checkEmail(char *name)
{
    int counter = 0;
    int i = 0;
    while( name[i] != '\0' ){
        if( name[i] == '@' ){
            counter++;
        }
        i++;
    }
    if(counter == 0 || counter > 1){
        return ROOMS_INVALID_PARAMETER;
    }

    return ROOMS_SUCCESS;
}

/**
 * Retrieves room ID
 * @param room
 * @return room id
 */
int getRoomId(Room room)
{
   assert(room != NULL);

    return room->id;
}

/**
 * Retrieves room price
 * @param room
 * @return room price
 */
int getRoomPrice(Room room)
{
    assert(room != NULL);
    return room->price;
}

/**
 * Retrieves room number of people recommended
 * @param room
 * @return room number of people recommended
 */
int getRoomNumOfPpl(Room room)
{
    assert(room != NULL);
    return room->num_of_ppl;
}

/**
 * Returns room calculation according to a given equation
 * @param room
 * @return calculation of equation
 */
double getRoomRecommendedCalculation(Room room,int P_e,int skill_level)
{
    return ( pow( (room->num_of_ppl - P_e), 2) + pow( (room->difficulty - skill_level), 2) );
}

/**
 * Retrieves room difficulty
 * @param room
 * @return room difficulty
 */
int getRoomDifficulty(Room room)
{
    assert(room != NULL);
    return room->difficulty;
}