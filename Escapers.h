#ifndef HW03_PART2_ESCAPERS_H
#define HW03_PART2_ESCAPERS_H

typedef struct SEscapers Escapers;

typedef enum {
    MTM_OUT_OF_MEMORY, // You should exit program after this error
    MTM_INVALID_COMMAND_LINE_PARAMETERS, // You should exit program after this error
    MTM_CANNOT_OPEN_FILE, // You should exit program after this error
    MTM_NULL_PARAMETER,
    MTM_INVALID_PARAMETER,
    MTM_EMAIL_ALREADY_EXISTS,
    MTM_COMPANY_EMAIL_DOES_NOT_EXIST,
    MTM_CLIENT_EMAIL_DOES_NOT_EXIST,
    MTM_ID_ALREADY_EXIST,
    MTM_ID_DOES_NOT_EXIST,
    MTM_CLIENT_IN_ROOM,
    MTM_ROOM_NOT_AVAILABLE,
    MTM_RESERVATION_EXISTS,
    MTM_NO_ROOMS_AVAILABLE,
    ESCAPER_SUCCESS,
} EscapersErrorCode;

#endif //HW03_PART2_ESCAPERS_H
