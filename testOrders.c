#include "test_utilities.h"
#include "mtm_ex3.h"
#include "Orders.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

static bool testcreateOrder()
{
    char *escaper_email = "nate@gmail.com";
    char *bad_email = "nate@@gmail.com";
    TechnionFaculty escaper_faculty = ELECTRICAL_ENGINEERING;
    TechnionFaculty company_faculty = ELECTRICAL_ENGINEERING;
    int room_id = 3, bad_id = -1;
    int req_num_of_ppl = 3, bad_req_num_of_ppl = -7;
    int req_hour = 13, bad_req_hour = 24;
    int req_day = 5, bad_req_day = -3;
    int room_price = 40, bad_room_price = -5;
    orderResult result;
    Order order = NULL;

    order = createOrder(escaper_email , escaper_faculty , company_faculty , room_id , req_num_of_ppl , req_hour ,
                        req_day , room_price , &result);
    ASSERT_TEST(result == ORDER_SUCCESS);
    ASSERT_TEST(order != NULL);
    freeOrder(order);

    order = createOrder(bad_email , escaper_faculty , company_faculty , room_id , req_num_of_ppl , req_hour ,
                        req_day , room_price , &result);
    ASSERT_TEST(result == ORDER_INVALID_PARAMETER);
    ASSERT_TEST(order == NULL);
    freeOrder(order);

    order = createOrder(escaper_email , escaper_faculty , company_faculty , bad_id , req_num_of_ppl , req_hour ,
                        req_day , room_price , &result);
    ASSERT_TEST(result == ORDER_INVALID_PARAMETER);
    ASSERT_TEST(order == NULL);
    freeOrder(order);

    order = createOrder(escaper_email , escaper_faculty , company_faculty , room_id , bad_req_num_of_ppl , req_hour ,
                        req_day , room_price , &result);
    ASSERT_TEST(result == ORDER_INVALID_PARAMETER);
    ASSERT_TEST(order == NULL);
    freeOrder(order);

    order = createOrder(escaper_email , escaper_faculty , company_faculty , room_id , req_num_of_ppl , bad_req_hour ,
                        req_day , room_price , &result);
    ASSERT_TEST(result == ORDER_INVALID_PARAMETER);
    ASSERT_TEST(order == NULL);
    freeOrder(order);

    order = createOrder(escaper_email , escaper_faculty , company_faculty , room_id , req_num_of_ppl , req_hour ,
                        bad_req_day , room_price , &result);
    ASSERT_TEST(result == ORDER_INVALID_PARAMETER);
    ASSERT_TEST(order == NULL);
    freeOrder(order);

    order = createOrder(escaper_email , escaper_faculty , company_faculty , room_id , req_num_of_ppl , req_hour ,
                        req_day , bad_room_price , &result);
    ASSERT_TEST(result == ORDER_INVALID_PARAMETER);
    ASSERT_TEST(order == NULL);
    freeOrder(order);


    return true;
}

static bool testcopyOrder()
{
    char *escaper_email = "nate@gmail.com";
    TechnionFaculty escaper_faculty = ELECTRICAL_ENGINEERING;
    TechnionFaculty company_faculty = ELECTRICAL_ENGINEERING;
    int room_id = 3;
    int req_num_of_ppl = 3;
    int req_hour = 13;
    int req_day = 5;
    int room_price = 40;
    int test = -1;
    char *str = NULL;
    orderResult result;
    Order order = NULL;
    Order copy_of_order = NULL;
    TechnionFaculty test_faculty = UNKNOWN;

    order = createOrder(escaper_email , escaper_faculty , company_faculty , room_id , req_num_of_ppl , req_hour ,
                        req_day , room_price , &result);
    ASSERT_TEST(result == ORDER_SUCCESS);
    ASSERT_TEST(order != NULL);

    copy_of_order = copyOrder(order);
    ASSERT_TEST(copy_of_order != NULL);
    test = getOrderDay(copy_of_order);
    ASSERT_TEST(test = req_day);
    test = getOrderHour(copy_of_order);
    ASSERT_TEST(test = req_hour);
    test = getOrderRoomId(copy_of_order);
    ASSERT_TEST(test = room_id);
    test = getOrderPrice(copy_of_order);
    ASSERT_TEST(test = room_price);
    str = getOrderEmail(copy_of_order);
    ASSERT_TEST(strcmp(str , escaper_email) == 0);
    test_faculty = getOrderRoomFaculty(order);
    ASSERT_TEST(test_faculty == company_faculty);

    freeOrder(order);
    freeOrder(copy_of_order);

    return true;
}

static bool testgetOrderDay() {
    char *escaper_email = "nate@gmail.com";
    TechnionFaculty escaper_faculty = ELECTRICAL_ENGINEERING;
    TechnionFaculty company_faculty = ELECTRICAL_ENGINEERING;
    int room_id = 3;
    int req_num_of_ppl = 3;
    int req_hour = 13;
    int req_day = 5;
    int room_price = 40;
    int test = -1;
    orderResult result;
    Order order = NULL;

    order = createOrder(escaper_email, escaper_faculty, company_faculty, room_id, req_num_of_ppl, req_hour,
                        req_day, room_price, &result);
    ASSERT_TEST(result == ORDER_SUCCESS);
    ASSERT_TEST(order != NULL);

    test = getOrderDay(order);
    ASSERT_TEST(test == req_day);

    freeOrder(order);

    return true;
}


static bool testincreaseOrderDay() {
    char *escaper_email = "nate@gmail.com";
    TechnionFaculty escaper_faculty = ELECTRICAL_ENGINEERING;
    TechnionFaculty company_faculty = ELECTRICAL_ENGINEERING;
    int room_id = 3;
    int req_num_of_ppl = 3;
    int req_hour = 13;
    int req_day = 5;
    int room_price = 40;
    int test = 0;
    orderResult result;
    Order order = NULL;

    order = createOrder(escaper_email, escaper_faculty, company_faculty, room_id, req_num_of_ppl, req_hour,
                        req_day, room_price, &result);
    ASSERT_TEST(result == ORDER_SUCCESS);
    ASSERT_TEST(order != NULL);

    increaseOrderDay(order);
    test = getOrderDay(order);
    ASSERT_TEST(test == req_day - 1);

    freeOrder(order);

    return true;
}

int main (int argv, char** arc){

    RUN_TEST(testcreateOrder);
    RUN_TEST(testcopyOrder);
    RUN_TEST(testgetOrderDay);
    RUN_TEST(testincreaseOrderDay);



    return 0;
}

//