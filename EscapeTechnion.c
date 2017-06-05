#include "EscapeTechnion.h"
#include "Companies.h"
#include "Escapers.h"
#include "Orders.h"

struct SEscapeTechnion{
    int day;
    Rooms *room;
    Escapers *escapers;
    Faculties *faculties;
    Companies *companies;
    Orders *orders;
};

struct SFaculty{
    TechnionFaculty faculty;
    Companies *company;
    int total_income;
};

