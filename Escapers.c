#include "Escapers.h"
#include "EscapeTechnion.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

struct SEscapers{
    char *email;
    TechnionFaculty faculty;
    int skill_level;
};

//free the inside of the escaper
void *freeEscaper(Escapers *escaper)
{
    free(escaper->email);
    escaper->email = NULL;
}

//copies an escaper element and returns a pointer to a new element
Escapers *copyElement(Escapers *escaper)
{
    Escapers *escaperCopy = malloc( sizeof(*escaperCopy) );
    if(escaperCopy == NULL){
        return NULL;
    }

    escaperCopy->email = malloc( strlen(escaper->email) + 1);
    if(escaperCopy->email == NULL){
        free(escaperCopy);
        return NULL;
    }
    strcpy(escaperCopy->email, escaper->email);

    escaperCopy->faculty = escaper->faculty;
    escaperCopy->skill_level = escaper->skill_level;

    return escaperCopy;
}