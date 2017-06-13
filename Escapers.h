#ifndef HW03_PART2_ESCAPERS_H
#define HW03_PART2_ESCAPERS_H

#include "mtm_ex3.h"
#include "set.h"

typedef struct SEscaper *Escaper;

typedef enum {
    ESCAPER_OUT_OF_MEMORY, // You should exit program after this error
    ESCAPER_NULL_PARAMETER,
    ESCAPER_INVALID_PARAMETER,
    ESCAPER_EMAIL_ALREADY_EXISTS,
    ESCAPER_COMPANY_EMAIL_DOES_NOT_EXIST,
    ESCAPER_CLIENT_EMAIL_DOES_NOT_EXIST,
    ESCAPER_SUCCESS,
} EscaperResult;

//free the inside of the escaper
void freeEscaper(Escaper escaper);

//free function to send to Set.h
void SetFreeEscaper(SetElement escaper);

int escaperCompare(Escaper escaper1, Escaper escaper2);

int SetEscaperCompare(SetElement escaper1, SetElement escaper2);

//copies an escaper element and returns a pointer to a new element
Escaper escaperCopyElement(Escaper escaper);

//copy function to send to the Set.h
SetElement setEscaperCopyElement(SetElement escaper);

Escaper createEscaper(char *name, TechnionFaculty faculty, int skill, EscaperResult *result);

int escaperGetSkill(Escaper escaper);

char *escaperGetEmail(Escaper escaper, EscaperResult *result);

TechnionFaculty escaperGetFaculty(Escaper escaper);

#endif //HW03_PART2_ESCAPERS_H
