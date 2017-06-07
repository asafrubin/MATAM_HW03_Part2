#ifndef HW03_PART2_ROOMS_H
#define HW03_PART2_ROOMS_H

typedef struct SRooms *Room;

typedef enum {
    ROOMS_OUT_OF_MEMORY, // You should exit program after this error
    ROOMS_INVALID_COMMAND_LINE_PARAMETERS, // You should exit program after this error
    ROOMS_CANNOT_OPEN_FILE, // You should exit program after this error
    ROOMS_NULL_PARAMETER,
    ROOMS_INVALID_PARAMETER,
    ROOMS_EMAIL_ALREADY_EXISTS,
    ROOMS_COMPANY_EMAIL_DOES_NOT_EXIST,
    ROOMS_CLIENT_EMAIL_DOES_NOT_EXIST,
    ROOMS_ID_ALREADY_EXIST,
    ROOMS_ID_DOES_NOT_EXIST,
    ROOMS_CLIENT_IN_ROOM,
    ROOMS_ROOM_NOT_AVAILABLE,
    ROOMS_RESERVATION_EXISTS,
    ROOMS_NO_ROOMS_AVAILABLE,
    ROOMS_SUCCESS,
} roomResult;

roomResult createRoom();

roomResult removeRoom(Room roomToRemove);

Room copyRoom(Room roomToCopy);

roomResult checkAvailable();

roomResult addEscaperToRoom();

roomResult sortRoom();

int compareRoom(Room firstRoom , Room secondRoom);

#endif //HW03_PART2_ROOMS_H
