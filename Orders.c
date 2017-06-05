#include "Orders.h"

struct SOrders{
    Escapers *escaper;
    Rooms *room;
    int req_hour;
    int req_num_of_ppl;
    int effective_ppl;
    int req_day;
};