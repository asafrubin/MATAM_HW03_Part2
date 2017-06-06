#include "Escapers.h"
#include "EscapeTechnion.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

struct SEscaper{
    char *email;
    TechnionFaculty faculty;
    int skill_level;
};

//free the inside of the escaper
void freeEscaper(Escaper *escaper)
{
    free(escaper->email);
    escaper->email = NULL;
}

//copies an escaper element and returns a pointer to a new element
Escaper *copyElement(Escaper *escaper)
{
    Escaper *escaperCopy = malloc( sizeof(*escaperCopy) );
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

EscaperResult createEscaper(char *name, TechnionFaculty faculty, int skill, Escaper *newEscaper)
{
    if(name == NULL){
        return ESCAPER_INVALID_PARAMETER;
    }
    if(skill < 1 || 10 < skill){
        return ESCAPER_INVALID_PARAMETER;
    }

    newEscaper = malloc(sizeof(Escaper));
    if(newEscaper == NULL){
        return ESCAPER_OUT_OF_MEMORY;
    }

    newEscaper->email = malloc( strlen(name) + 1 );
    if(newEscaper->email == NULL){
        free(newEscaper);
        return ESCAPER_OUT_OF_MEMORY;
    }
    strcpy(newEscaper->email, name);
    newEscaper->skill_level = skill;
    newEscaper->faculty = faculty;

    return ESCAPER_SUCCESS;
}

void removeEscaper(Escaper *escaper)
{
    if(escaper == NULL){
        retrun;
    }
    free(escaper->email);
    free(escaper);
}