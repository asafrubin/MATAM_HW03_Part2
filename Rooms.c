#include "Rooms.h"
#include <string.h>
#include <stdlib.h>

typedef struct SRooms{
    char *email;
    int id;
    int price;
    int num_of_ppl;
    int time_start;
    int time_finish;
    int difficulty;
} *Room;

//requires that PARSER pass on deconstucted working_hrs
roomResult createRoom(char *email , int id , int price , int num_ppl , int time_start , int time_finish , int difficulty
                      , Room newRoom)
{
    newRoom = malloc(sizeof(*newRoom));
    if (newRoom == NULL){
        return ROOMS_OUT_OF_MEMORY;
    }
    newRoom->email = malloc(strlen(email) + 1);
    if (newRoom->email == NULL){
        free(newRoom);
        return ROOMS_OUT_OF_MEMORY;
    }
    strcpy(newRoom->email , email);
    newRoom->id = id;
    newRoom->price = price;
    newRoom->num_of_ppl = num_ppl;
    newRoom->time_start = time_start;
    newRoom->time_finish = time_finish;
    newRoom->difficulty = difficulty;
    return ROOMS_SUCCESS;
}

Room copyRoom(Room roomToCopy)
{
    Room copyOfRoom = NULL;
    createRoom(roomToCopy->email , roomToCopy->id , roomToCopy->price , roomToCopy->num_of_ppl , roomToCopy->time_start
            , roomToCopy->time_finish , roomToCopy->difficulty , copyOfRoom);

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