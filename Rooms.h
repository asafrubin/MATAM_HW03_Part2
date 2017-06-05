#ifndef HW03_PART2_ROOMS_H
#define HW03_PART2_ROOMS_H

#include "EscapeTechnion.h"

typedef struct SRooms Rooms;

typedef enum roomResult_t {
    //SET_SUCCESS,
    ROOM_OUT_OF_MEMORY,
    ROOM_NULL_ARGUMENT,
    //SET_ITEM_ALREADY_EXISTS,
    //SET_ITEM_DOES_NOT_EXIST

} RoomResult;

RoomResult createRoom();

RoomResult freeRoom();

RoomResult copyRoom();

RoomResult checkAvailable();

RoomResult addEscaperToRoom();

RoomResult sortRoom();

RoomResult

#endif //HW03_PART2_ROOMS_H
