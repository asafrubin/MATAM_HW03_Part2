#include "Orders.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"

#define DISCOUNT 0.75

struct SOrders{
    char *escaperEmail;
    TechnionFaculty companyFaculty;
    TechnionFaculty escaperFaculty;
    int roomID;
    int req_num_of_ppl;
    int req_hour;
    int req_day;
    int price;
};

static orderResult checkEmail(char *name)
{
    if(name == NULL){
        return ORDER_INVALID_PARAMETER;
    }
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
static int calculatePrice(bool discount , int roomPrice)
{
    if(discount){
        roomPrice = (DISCOUNT * roomPrice); //roomPrice will always be multiplies of 4
    }
    return roomPrice;
}

Order createOrder(char *escaperEmail, TechnionFaculty escaperFaculty, TechnionFaculty companyFaculty, int roomID,
                  int req_num_of_ppl, int req_hour, int req_day, int roomPrice, orderResult *result)
{
    Order newOrder;
    bool discount;

    if(req_num_of_ppl <= 0 || roomPrice < 0 || req_day < 0 || req_hour < 0 || req_hour > 23){
        *result = ORDER_INVALID_PARAMETER;
        return NULL;
    }
    if( checkEmail(escaperEmail) != ORDER_SUCCESS ){
        *result =  ORDER_INVALID_PARAMETER;
        return NULL;
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


    //set discount to true if escaper gets a discount
    discount = (escaperFaculty == companyFaculty);

    newOrder->escaperFaculty = escaperFaculty;
    newOrder->companyFaculty = companyFaculty;
    newOrder->roomID = roomID;
    newOrder->req_num_of_ppl = req_num_of_ppl;
    newOrder->req_hour = req_hour;
    newOrder->req_day = req_day;
    newOrder->price = calculatePrice(discount , roomPrice);

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
    Order copy_of_order = malloc(sizeof(*copy_of_order));
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
    copy_of_order->companyEmail = malloc(strlen(order_to_copy->companyEmail) + 1);
    if(copy_of_order->companyEmail == NULL){
        free(copy_of_order->escaperEmail);
        free(copy_of_order);
        return NULL;
    }
    strcpy(copy_of_order->companyEmail , order_to_copy->companyEmail);
    */
    copy_of_order->roomID = order_to_copy->roomID;
    copy_of_order->req_num_of_ppl = order_to_copy->req_num_of_ppl;
    copy_of_order->req_hour = order_to_copy->req_hour;
    copy_of_order->req_day = order_to_copy->req_day;
    copy_of_order->price = order_to_copy->price;
    copy_of_order->companyFaculty = order_to_copy->companyFaculty;
    copy_of_order->escaperFaculty = order_to_copy->escaperFaculty;

    return copy_of_order;
}

ListElement listCopyOrder(ListElement order_to_copy)
{
    return copyOrder( order_to_copy );
}

/**
 * Removes a single DATA TYPE of Order
 * @param order : pointer to the Order to be removed
 * @return : Error Enum
 */
void freeOrder(Order order)
{
    if(order) {
        free(order->escaperEmail);
        free(order);
    }
}

void listFreeOrder(ListElement order)
{
    freeOrder(order);
}

/**
 * Used to get the price from a created order
 * @param order : pointer to order inquired
 * @return : price of order inquired
 */
int getOrderPrice(Order order)
{
    assert(order != NULL);
    return order->price;
}

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

TechnionFaculty getOrderRoomFaculty(Order order)
{
    assert(order != NULL);
    return order->companyFaculty;
}

void increaseOrderDay(Order order)
{
    order->req_day--;
}

bool orderDayArrived(ListElement order, ListFilterKey key)
{
    Order checkOrder = order;

    if(checkOrder->req_day < *(int*)(key) ){
        return true;
    }

    return false;
}

bool orderDayNotArrived(ListElement order, ListFilterKey key)
{
    Order checkOrder = order;

    if(checkOrder->req_day >= *(int*)(key) ){
        return true;
    }

    return false;
}

List createOrderDayNotArrivedFilteredList(List ListOfOrders)
{
    int key = 0;
    return listFilter(ListOfOrders, orderDayNotArrived, &key);
}

List createOrderDayArrivedFilteredList(List ListOfOrders)
{
    int key = 0;
    return listFilter(ListOfOrders, orderDayArrived, &key);
}

int hourOfDay(ListElement order1, ListElement order2)
{

    Order firstOrder = order1, secondOrder = order2;

    return secondOrder->req_hour - firstOrder->req_hour;
}

void sortOrdersByHour(List orders)
{
    listSort(orders, hourOfDay);
}

void printOrder(FILE *outputStream, Order order, int escaperSkill, int roomDifficulty, char *companyEmail)
{
    mtmPrintOrder(outputStream, order->escaperEmail, escaperSkill ,order->escaperFaculty, companyEmail,
                  order->companyFaculty, order->roomID, order->req_hour, roomDifficulty,
                  order->req_num_of_ppl, order->price);
}