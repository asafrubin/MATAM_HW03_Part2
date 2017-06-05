#include "EscapeTechnion.h"
#include "Companies.h"
#include "Escapers.h"
#include "Orders.h"
#include "Rooms.h"

struct SEscapeTechnion{
    int day;
    List rooms;
    List escapers;
    List faculties;
    Set companies;
    List orders;
};

struct SFaculty{
    TechnionFaculty faculty;
    Companies *company;
    int total_income;
};