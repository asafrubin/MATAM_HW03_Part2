#include "Rooms.h"
#include <string.h>

typedef struct SRooms{
    char *email;
    int id;
    int price;
    int num_of_ppl;
    int time_start;
    int time_finish;
    int difficulty;
//    Companies *company;
} Room;


//requires that PARSER pass on deconstucted working_hrs
RoomResult createRoom(char *email , int id , int price , int num_ppl , int time_start , int time_finish , int difficulty , Room *newRoom)
{
    Room *newRoom = malloc(sizeof(Room));
    if (newRoom == NULL){
        return ROOM_OUT_OF_MEMORY;
    }
    newRoom->email = malloc(strlen(email));
    if (newRoom->email == NULL){
        return ROOM_OUT_OF_MEMORY;
    }
    strcpy(newRoom->email , email);
    newRoom->id = id;
    newRoom->price = price;
    newRoom->num_of_ppl = num_ppl;
    newRoom->time_start = time_start;
    newRoom->time_finish = time_finish;
    newRoom->difficulty = difficulty;
}

