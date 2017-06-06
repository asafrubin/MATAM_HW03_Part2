#include "EscapeTechnion.h"
#include "Companies.h"
#include "Escapers.h"
#include "Orders.h"
#include "Rooms.h"
#include <stdlib.h>

struct SEscapeTechnion{
    int day;
    Set rooms;
    Set escapers;
    List faculties;
    Set companies;
    List orders;
};


MtmErrorCode escaperAdd(char *email, TechnionFaculty faculty, int skill, Set escapers)
{
    EscaperResult escaperResult;
    SetResult setResult;
    Escaper *newEscaper = NULL;

    escaperResult = createEscaper(email, faculty, skill, newEscaper);
    if(escaperResult != ESCAPER_SUCCESS){
        return (MtmErrorCode)escaperResult; // need to write a function to convert
    }

    setResult = setAdd(escapers, newEscaper);
    destroyEscaper(newEscaper);

    if(setResult != SET_SUCCESS){
        return (MtmErrorCode)setResult;
    }

    return MTM_SUCCESS;
}

