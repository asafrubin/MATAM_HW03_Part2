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

//requires that PARSER pass on deconstucted working_hrs
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

roomResult roomCheckIfOpen(Room room, int req_hour)
{
    assert(room != NULL);
    if(room->time_start <= req_hour && req_hour < room->time_finish){
        return ROOMS_SUCCESS;
    }

    return ROOMS_ROOM_NOT_AVAILABLE;
}

Room copyRoom(Room roomToCopy)
{
    Room copyOfRoom = NULL;
    roomResult result;

    copyOfRoom = createRoom(roomToCopy->email , roomToCopy->id , roomToCopy->price, roomToCopy->num_of_ppl,
                            roomToCopy->time_start, roomToCopy->time_finish , roomToCopy->difficulty , &result);

    return copyOfRoom;
}

void removeRoom(Room room)
{
    if(room){
        assert(room->email != NULL);
        free(room->email);
        free(room);
        }

}

int compareRoom(Room firstRoom, Room secondRoom)
{
    return ((firstRoom->id) - (secondRoom->id));
}

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

int getRoomId(Room room)
{
    if(room == NULL){
        return -1;
    }

    return room->id;
}

int getRoomPrice(Room room)
{
    assert(room != NULL);
    return room->price;
}

int getRoomNumOfPpl(Room room)
{
    assert(room != NULL);
    return room->num_of_ppl;
}

double getRoomRecommendedCalculation(Room room,int P_e,int skill_level)
{
    return ( pow( (room->num_of_ppl - P_e), 2) + pow( (room->difficulty - skill_level), 2) );
}

int getRoomDifficulty(Room room)
{
    assert(room != NULL);
    return room->difficulty;
}