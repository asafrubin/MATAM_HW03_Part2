#ifndef HW03_PART2_ROOMS_H
#define HW03_PART2_ROOMS_H

#define MIN_OPEN_TIME 0
#define MAX_OPEN_TIME 24
#define MIN_DIFFICULTY 0
#define MAX_DIFFICULTY 10


typedef struct SRooms *Room;

typedef enum {
    ROOMS_OUT_OF_MEMORY, // You should exit program after this error
    ROOMS_INVALID_COMMAND_LINE_PARAMETERS, // You should exit program after this error
    ROOMS_CANNOT_OPEN_FILE, // You should exit program after this error
    ROOMS_NULL_PARAMETER,
    ROOMS_INVALID_PARAMETER,
    ROOMS_ID_ALREADY_EXIST,
    ROOMS_ID_DOES_NOT_EXIST,
    ROOMS_CLIENT_IN_ROOM,
    ROOMS_ROOM_NOT_AVAILABLE,
    ROOMS_RESERVATION_EXISTS,
    ROOMS_NO_ROOMS_AVAILABLE,
    ROOMS_SUCCESS,
} roomResult;

Room createRoom(char *email, int id, int price, int num_ppl, int time_start, int time_finish, int difficulty
        ,roomResult *result);

void removeRoom(Room roomToRemove);

Room copyRoom(Room roomToCopy);

int getRoomId(Room room);

int getRoomPrice(Room room);

int getRoomNumOfPpl(Room room);

int getRoomDifficulty(Room room);

roomResult checkRoomParameters(char *email, int id, int price, int num_ppl,
                               int time_start, int time_finish, int difficulty);

double getRoomRecommendedCalculation(Room room, int P_e, int skill_level);

roomResult roomCheckIfOpen(Room room, int req_hour);

int compareRoom(Room firstRoom, Room secondRoom);

#endif //HW03_PART2_ROOMS_H
