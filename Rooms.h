#ifndef HW03_PART2_ROOMS_H
#define HW03_PART2_ROOMS_H

#define MIN_OPEN_TIME 0
#define MAX_OPEN_TIME 24
#define MIN_DIFFICULTY 0
#define MAX_DIFFICULTY 10


typedef struct SRooms *Room;

typedef enum {
    ROOMS_OUT_OF_MEMORY, // You should exit program after this error
    ROOMS_NULL_PARAMETER,
    ROOMS_INVALID_PARAMETER,
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

roomResult checkAvailable();

roomResult addEscaperToRoom();

roomResult sortRoom();

int compareRoom(Room firstRoom, Room secondRoom);

int getRoomPrice(Room room);

void* setCopyRoom(void* roomToCopy);

void setRemoveRoom(void* roomToRemove);

int setCompareRoom(void* firstRoom , void* secondRoom);

#endif //HW03_PART2_ROOMS_H
