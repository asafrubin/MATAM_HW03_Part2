#include "Orders.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DISCOUNT 0.25

struct SOrders{
    char *escaperEmail;
    //char *facultyEmail;
    TechnionFaculty faculty;
    int roomID;
    int req_num_of_ppl;
    //int effective_ppl;
    int req_hour;
    int req_day;
    //double price;
};

static orderResult checkEmail(char *name)
{
    int counter = 0;
    int i = 0;
    while( name[i] != '\0' ){
        if( name[i] == '@' ){
            counter++;
        }
        i++;
    }
    if(counter == 0 || counter > 1){
        return ORDER_INVALID_PARAMETER;
    }

    return ORDER_SUCCESS;
}

/**
 * Used to calculate price (check for discount) when creating an order
 * @param discount : true is eligabile for a discount
 * @param roomPrice : standard price of the room ordered
 * @return : the price to charged from the client
 */

/*
static double calculatePrice(bool discount , double roomPrice)
{
    double price = roomPrice;
    if(discount){
        price = (1 - DISCOUNT) * price;
    }
    return price;
}
*/

Order createOrder(char *escaperEmail, TechnionFaculty faculty, int roomID , int req_num_of_ppl ,
                        int req_hour , int req_day, int system_day, orderResult *result)
{
    Order newOrder;

    if(req_num_of_ppl < 0){
        *result = ORDER_INVALID_PARAMETER;
    }
    if(req_hour < 0 || req_hour > 23){
        *result = ORDER_INVALID_PARAMETER;
    }
    if( req_day < 0){
        *result =  ORDER_INVALID_PARAMETER;
    }
    if( checkEmail(escaperEmail) != ORDER_SUCCESS){
        *result =  ORDER_INVALID_PARAMETER;
    }

    newOrder = malloc(sizeof(*newOrder));
    if(newOrder == NULL){
        *result = ORDER_OUT_OF_MEMORY;
        return NULL;
    }
    newOrder->escaperEmail = malloc(strlen(escaperEmail) + 1);
    if(newOrder->escaperEmail == NULL){
        free(newOrder);
        *result = ORDER_OUT_OF_MEMORY;
        return NULL;
    }
    strcpy(newOrder->escaperEmail , escaperEmail);
    /*
    newOrder->facultyEmail = malloc(strlen(facultyEmail) + 1);
    if(newOrder->facultyEmail == NULL){
        free(newOrder->escaperEmail);
        free(newOrder);
        *result = ORDER_OUT_OF_MEMORY;
        return NULL;
    }
     */
    newOrder->faculty = faculty;
    newOrder->roomID = roomID;
    newOrder->req_num_of_ppl = req_num_of_ppl;
    //newOrder->effective_ppl = effective_ppl;
    newOrder->req_hour = req_hour;
    newOrder->req_day = req_day - systemDay;
    //newOrder->price = calculatePrice(discount , roomPrice);

    *result = ORDER_SUCCESS;

    return newOrder;
}

/**
 * Creates a pointer to a copy of an Order inserted into the function
 * @param order_to_copy : pointer to order to be copied
 * @return : pointer to newly created order
 */
Order copyOrder(Order order_to_copy)
{
    Order copy_of_order = malloc(sizeof(Order));
    if(copy_of_order == NULL){
        return NULL;
    }
    copy_of_order->escaperEmail = malloc(strlen(order_to_copy->escaperEmail) + 1);
    if(copy_of_order->escaperEmail == NULL){
        free(copy_of_order);
        return NULL;
    }
    strcpy(copy_of_order->escaperEmail , order_to_copy->escaperEmail);
    /*
    copy_of_order->facultyEmail = malloc(strlen(order_to_copy->facultyEmail) + 1);
    if(copy_of_order->facultyEmail == NULL){
        free(copy_of_order->escaperEmail);
        free(copy_of_order);
        return NULL;
    }
    strcpy(copy_of_order->facultyEmail , order_to_copy->facultyEmail);
     */
    copy_of_order->roomID = order_to_copy->roomID;
    copy_of_order->req_num_of_ppl = order_to_copy->req_num_of_ppl;
    //copy_of_order->effective_ppl = order_to_copy->effective_ppl;
    copy_of_order->req_hour = order_to_copy->req_hour;
    copy_of_order->req_day = order_to_copy->req_day;
    //copy_of_order->price = order_to_copy->price;
    return copy_of_order;
}

/**
 * Removes a single DATA TYPE of Order
 * @param order : pointer to the Order to be removed
 * @return : Error Enum
 */
orderResult freeOrder(Order order)
{
    //free(order->facultyEmail);
    free(order->escaperEmail);
    free(order);
    return ORDER_SUCCESS;
}

/**
 * Used to get the price from a created order
 * @param order : pointer to order inquired
 * @return : price of order inquired
 */

/*
double getOrderPrice(Order order)
{
    return order->price;
}
*/

int getOrderDay(Order order)
{
    if(order == NULL){
        return -1;
    }

    return order->req_day;
}

int getOrderHour(Order order)
{
    if(order == NULL){
        return -1;
    }

    return order->req_hour;
}

int getOrderRoomId(Order order)
{
    if(order == NULL){
        return -1;
    }

    return order->roomID;
}

char *getOrderEmail(Order order)
{
    char *email = NULL;

    if(order == NULL){
        assert(order != NULL);
        return NULL;
    }
    email = malloc( strlen(order->escaperEmail) + 1);
    if(email == NULL){
        return NULL;
    }
    strcpy(email, order->escaperEmail);

    return email;
}

//need fix it
TechnionFaculty getOrderFaculty(Order order)
{
    return order->faculty;
}

void increaseOrderDay(Order order)
{
    order->req_day--;
}