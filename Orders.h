#ifndef HW03_PART2_ORDERS_H
#define HW03_PART2_ORDERS_H

#include "EscapeTechnion.h"

typedef struct SOrders *Order;

typedef enum {
    ORDER_OUT_OF_MEMORY, // You should exit program after this error
    ORDER_INVALID_COMMAND_LINE_PARAMETERS, // You should exit program after this error
    ORDER_CANNOT_OPEN_FILE, // You should exit program after this error
    ORDER_NULL_PARAMETER,
    ORDER_INVALID_PARAMETER,
    ORDER_EMAIL_ALREADY_EXISTS,
    ORDER_COMPANY_EMAIL_DOES_NOT_EXIST,
    ORDER_CLIENT_EMAIL_DOES_NOT_EXIST,
    ORDER_ID_ALREADY_EXIST,
    ORDER_ID_DOES_NOT_EXIST,
    ORDER_CLIENT_IN_ROOM,
    ORDER_ROOM_NOT_AVAILABLE,
    ORDER_RESERVATION_EXISTS,
    ORDER_NO_ROOMS_AVAILABLE,
    ORDER_SUCCESS,
} orderResult;

Order createOrder(char *escaperEmail, TechnionFaculty faculty, int roomID , int req_num_of_ppl ,
                  int req_hour , int req_day, int system_day, orderResult *result);

orderResult freeOrder(Order order);

int getOrderDay(Order order);

int getOrderHour(Order order);

int getOrderRoomId(Order order);

char *getOrderEmail(Order order);

void increaseOrderDay(Order order);

TechnionFaculty getOrderFaculty(Order order);



#endif //HW03_PART2_ORDERS_H
