#include "Orders.h"
#include <stdbool.h>
#include <stdlib.h>

struct SOrders{
    char *escaperEmail;
    char *facultyEmail;
    int roomID;
    int req_hour;
    int req_num_of_ppl;
    int effective_ppl;
    int req_day;
    bool discount;
};

orderResult createOrder(char* escaperEmail , )