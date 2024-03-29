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
static int calculatePrice(bool discount ,int roomPrice, int req_num_of_ppl)
{
    roomPrice *= req_num_of_ppl;
    if(discount){
        roomPrice = (DISCOUNT * roomPrice); //roomPrice will always be multiplies of 4
    }
    return roomPrice;
}

/**
 * Creates a new Element of order type
 * @param escaperEmail : email of escaper who requested order
 * @param escaperFaculty : faculty of escaper
 * @param companyFaculty : faculty to which the room belongs
 * @param roomID : unique id of room
 * @param req_num_of_ppl : number of people to enter room
 * @param req_hour : hour in which customer will enter room
 * @param req_day : day in which customer will enter room
 * @param roomPrice : price to be payed
 * @param result : orderResult (ENUM)
 * @return : newly created order
 */
Order createOrder(char *escaperEmail, TechnionFaculty escaperFaculty, TechnionFaculty companyFaculty, int roomID,
                  int req_num_of_ppl, int req_hour, int req_day, int roomPrice, orderResult *result)
{
    Order newOrder;
    bool discount;

    if(req_num_of_ppl <= 0 || roomPrice < 0 || req_day < 0 || req_hour < 0 || req_hour > 23 || roomID < 0){
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
    newOrder->price = calculatePrice(discount , roomPrice, req_num_of_ppl);

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

/**
 * Aux function used by list.h
 * @param order_to_copy
 * @return
 */
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

/**
 * Aux function used by list.h
 * @param order_to_copy
 * @return
 */
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

/**
 * Used to get the day from a created order
 * @param order : pointer to order inquired
 * @return : day of order inquired
 */
int getOrderDay(Order order)
{
    if(order == NULL){
        return -1;
    }

    return order->req_day;
}

/**
 * Used to get the hour from a created order
 * @param order : pointer to order inquired
 * @return : hour of order inquired
 */
int getOrderHour(Order order)
{
    if(order == NULL){
        return -1;
    }

    return order->req_hour;
}

/**
 * Used to get the roomID from a created order
 * @param order : pointer to order inquired
 * @return : roomID of order inquired
 */
int getOrderRoomId(Order order)
{
    if(order == NULL){
        return -1;
    }

    return order->roomID;
}

/**
 * Used to get the escaper email from a created order
 * @param order : pointer to order inquired
 * @return : escaper email of order inquired
 */
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

/**
 * Used to get the faculty which owns the room from a created order
 * @param order : pointer to order inquired
 * @return : faculty which owns the room of order inquired
 */
TechnionFaculty getOrderRoomFaculty(Order order)
{
    assert(order != NULL);
    return order->companyFaculty;
}

/**
 * Used to bring order closer to current day --(days left to order)
 * @param order : order which to apply to
 */
void increaseOrderDay(Order order)
{
    assert(order != NULL);
    order->req_day--;
}

/**
 * Compares a filter key of orders with requested day
 * @param order : order which to apply to
 * @param key : key used by list.h
 * @return : if inquired order matches key
 */
bool orderDayArrived(ListElement order, ListFilterKey key)
{
    Order checkOrder = order;

    if(checkOrder->req_day < *(int*)(key) ){
        return true;
    }

    return false;
}

/**
 * Compares a filter key of orders with requested day
 * @param order : order which to apply to
 * @param key : key used by list.h
 * @return : if inquired order matches key
 */
bool orderDayNotArrived(ListElement order, ListFilterKey key)
{
    Order checkOrder = order;

    if(checkOrder->req_day >= *(int*)(key) ){
        return true;
    }

    return false;
}

/**
 * Creates a list of orders matching a certain key (requested day >= 0)
 * @param ListOfOrders : list of orders
 * @return : new list matching key requirements
 */
List createOrderDayNotArrivedFilteredList(List ListOfOrders)
{
    int key = 0;
    return listFilter(ListOfOrders, orderDayNotArrived, &key);
}

/**
 * Creates a list of orders matching a certain key (requested day < 0)
 * @param ListOfOrders : list of orders
 * @return : new list matching key requirements
 */
List createOrderDayArrivedFilteredList(List ListOfOrders)
{
    int key = 0;
    return listFilter(ListOfOrders, orderDayArrived, &key);
}

/**
 * Used to find order
 * @param order1
 * @param order2
 * @return
 */
int hourOfDay(ListElement order1, ListElement order2)
{

    int result;
    Order firstOrder = order1, secondOrder = order2;

    result =  (firstOrder->req_hour - secondOrder->req_hour);
    if(result == 0){
        result = (firstOrder->companyFaculty - secondOrder->companyFaculty);
        if(result == 0){
            return (firstOrder->roomID - secondOrder->roomID);
        }
    }
    return result;
}

/**
 * Reorganizes list of orders by hour
 * @param orders : list of orders
 */
void sortOrdersByHour(List orders)
{
    listSort(orders, hourOfDay);
}

/**
 * Prints order to output stream
 * @param outputStream : pointer to output stream
 * @param order : order to print
 * @param escaperSkill : skill of escaper
 * @param roomDifficulty : dificulty of room
 * @param companyEmail : email of company
 */
void printOrder(FILE *outputStream, Order order, int escaperSkill, int roomDifficulty, char *companyEmail)
{
    mtmPrintOrder(outputStream, order->escaperEmail, escaperSkill ,order->escaperFaculty, companyEmail,
                  order->companyFaculty, order->roomID, order->req_hour, roomDifficulty,
                  order->req_num_of_ppl, order->price);
}

void removeOrderOfEscaper(List orders, char *email)
{

    LIST_FOREACH(Order,order, orders){
        if(strcmp(order->escaperEmail, email) == 0){
            listRemoveCurrent(orders);
            listGetFirst(orders);
        }
    }
}