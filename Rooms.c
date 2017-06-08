#include "Rooms.h"
#include <string.h>
#include <stdlib.h>

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
    if(time_start < MIN_OPEN_TIME || time_start > MAX_OPEN_TIME || time_finish < MIN_OPEN_TIME || time_finish > MAX_OPEN_TIME){
        *result = ROOMS_INVALID_PARAMETER;
        return NULL;
    }

    if(difficulty < MIN_DIFFICULTY || difficulty > MAX_DIFFICULTY){
        *result = ROOMS_INVALID_PARAMETER;
        return NULL;
    }

    if( checkEmail(email) != ROOMS_SUCCESS ){
        *result = ROOMS_INVALID_PARAMETER;
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

    *result =  ROOMS_SUCCESS;

    return newRoom;
}

Room copyRoom(Room roomToCopy)
{
    Room copyOfRoom = NULL;
    roomResult result;

    copyOfRoom = createRoom(roomToCopy->email , roomToCopy->id , roomToCopy->price, roomToCopy->num_of_ppl,
                            roomToCopy->time_start, roomToCopy->time_finish , roomToCopy->difficulty , &result);

    return copyOfRoom;
}

roomResult removeRoom(Room room)
{
    //Check if this test is relavent
    if(room == NULL){
        return ROOMS_NULL_PARAMETER;
    }
    free(room->email);
    free(room);
    return ROOMS_SUCCESS;
}

int compareRoom(Room firstRoom , Room secondRoom)
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