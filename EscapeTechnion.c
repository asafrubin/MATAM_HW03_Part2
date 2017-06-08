#include "EscapeTechnion.h"
#include "Companies.h"
#include "Escapers.h"
#include "Orders.h"
#include "Rooms.h"
#include <stdlib.h>

typedef struct SEscapeTechnion{
    FILE *outputStream;
    int day;
    Set rooms;
    Set escapers;
    Set companies;
    List orders;
};


MtmErrorCode mtmEscaperAdd(char *email, TechnionFaculty faculty, int skill, EscapeTechnion *escapeTechnion)
{
    EscaperResult escaperResult;
    SetResult setResult;
    Escaper *newEscaper = NULL;

    escaperResult = createEscaper(email, faculty, skill, newEscaper);
    if(escaperResult != ESCAPER_SUCCESS){
        return (MtmErrorCode)escaperResult; // need to write a function to convert
    }

    setResult = setAdd( (*escapeTechnion)->escapers, newEscaper);
    removeEscaper(newEscaper);

    if(setResult != SET_SUCCESS){
        return (MtmErrorCode)setResult;
    }

    return MTM_SUCCESS;
}


