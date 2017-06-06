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
    int price;
} Order;

orderResult createOrder(char* escaperEmail , char *facultyEmail , int roomID , int req_num_of_ppl , int effective_ppl ,
                        int req_hour , int req_day , int price , Order *newOrder)
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
    newOrder->price = price;
    return ORDERS_SUCCESS;
}

Order* copyOrder(Order order_to_copy)
{
    Order *copy_of_order = malloc(sizeof(Order));
    if(copy_of_order == NULL){
        return ORDERS_OUT_OF_MEMORY;
    }
    copy_of_order->escaperEmail = malloc(strlen(order_to_copy.escaperEmail) + 1);
    if(copy_of_order->escaperEmail == NULL){
        free(copy_of_order);
        return ORDERS_OUT_OF_MEMORY;
    }
    strcpy(copy_of_order->escaperEmail , order_to_copy.escaperEmail);
    copy_of_order->facultyEmail = malloc(strlen(order_to_copy.facultyEmail) + 1);
    if(copy_of_order->facultyEmail == NULL){
        free(copy_of_order->escaperEmail);
        free(copy_of_order);
        return ORDERS_OUT_OF_MEMORY;
    }
    strcpy(copy_of_order->facultyEmail , order_to_copy.facultyEmail);
    copy_of_order->roomID = order_to_copy.roomID;
    copy_of_order->req_num_of_ppl = order_to_copy.req_num_of_ppl;
    copy_of_order->effective_ppl = order_to_copy.effective_ppl;
    copy_of_order->req_hour = order_to_copy.req_hour;
    copy_of_order->req_day = order_to_copy.req_day;
    copy_of_order->price = order_to_copy.price;
    return copy_of_order;
}

orderResult removeOrder(Order *order)
{
    free(order->facultyEmail);
    free(order->escaperEmail);
    free(order);
    return ORDERS_SUCCESS;
}

int getPrice(Order *order)
{
    return order->price;
}