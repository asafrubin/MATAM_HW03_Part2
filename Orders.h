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

Order createOrder(char *escaperEmail,TechnionFaculty escaperFaculty, TechnionFaculty companyFaculty, int roomID,
                  int req_num_of_ppl, int req_hour, int req_day, int system_day, int roomPrice, orderResult *result);

Order copyOrder(Order order_to_copy);

ListElement listCopyOrder(ListElement order_to_copy);

void freeOrder(Order order);

void listFreeOrder(ListElement order);

int getOrderDay(Order order);

int getOrderHour(Order order);

int getOrderRoomId(Order order);

int getOrderPrice(Order order);

char *getOrderEmail(Order order);

void increaseOrderDay(Order order);

List createOrderDayArrivedFilteredList(List ListOfOrders);

List createOrderDayNotArrivedFilteredList(List ListOfOrders);

void sortOrdersByHour(List orders);

TechnionFaculty getOrderRoomFaculty(Order order);

void printOrder(FILE *outputStream, Order order, int escaperSkill, int roomDifficulty, char *companyEmail);



#endif //HW03_PART2_ORDERS_H
