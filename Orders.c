#include "Orders.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct SOrders{
    char *escaperEmail;
    char *facultyEmail;
    int roomID;
    int req_num_of_ppl;
    int effective_ppl;
    int req_hour;
    int req_day;
    bool discount;
} Order;

orderResult createOrder(char* escaperEmail , char *facultyEmail , int roomID , int req_num_of_ppl , int effective_ppl ,
                        int req_hour , int req_day , bool discount , Order *newOrder)
{
    newOrder = malloc(sizeof(Order));
    if(newOrder == NULL){
        return ORDERS_OUT_OF_MEMORY;
    }
    newOrder->escaperEmail = malloc(strlen(escaperEmail) + 1);
    if(newOrder->escaperEmail == NULL){
        free(newOrder);
        return ORDERS_OUT_OF_MEMORY;
    }
    strcpy(newOrder->escaperEmail , escaperEmail);
    newOrder->facultyEmail = malloc(strlen(facultyEmail) + 1);
    if(newOrder->facultyEmail == NULL){
        free(newOrder->escaperEmail);
        free(newOrder);
        return ORDERS_OUT_OF_MEMORY;
    }
    newOrder->roomID = roomID;
    newOrder->req_num_of_ppl = req_num_of_ppl;
    newOrder->effective_ppl = effective_ppl;
    newOrder->req_hour = req_hour;
    newOrder->req_day = req_day;
    newOrder->discount = discount;
    return ORDERS_SUCCESS;
}

Order* copyOrder(Order order_to_copy)
{
    Order *copy_of_order = malloc(sizeof(Order));
    if(copy_of_order == NULL){
        return ORDERS_OUT_OF_MEMORY;
    }
    copy_of_order->escaperEmail = malloc(strlen(order_to_copy.escaperEmail));
    if(copy_of_order->escaperEmail == NULL){
        free(copy_of_order);
        return ORDERS_OUT_OF_MEMORY;
    }
    strcpy(copy_of_order->escaperEmail , order_to_copy.escaperEmail);

}