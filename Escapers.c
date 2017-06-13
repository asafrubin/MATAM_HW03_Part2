#include "Escapers.h"
#include "set.h"
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

EscaperResult static checkEmail(char *name);

struct SEscaper{
    char *email;
    TechnionFaculty faculty;
    int skill_level;
};

//free the inside of the escaper
void freeEscaper(Escaper escaper)
{
    if(escaper) {
        free(escaper->email);
        escaper->email = NULL;
        free(escaper);
        escaper = NULL;
    }
}

void SetFreeEscaper(SetElement escaper)
{
    freeEscaper( (SetElement)escaper );
}

//copies an escaper element and returns a pointer to a new element
Escaper escaperCopyElement(Escaper escaper)
{
    Escaper escaperCopy = NULL;

    if(escaper == NULL){
        return NULL;
    }
    escaperCopy = malloc( sizeof(*escaperCopy) );
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

SetElement setEscaperCopyElement(SetElement escaper)
{
    return (SetElement)escaperCopyElement( (Escaper)escaper );
}

int escaperCompare(Escaper escaper1, Escaper escaper2)
{
    assert(escaper1 != NULL);
    assert(escaper2 != NULL);
    return strcmp( escaper1->email, escaper2->email);
}

int SetEscaperCompare(SetElement escaper1, SetElement escaper2)
{
    return escaperCompare( (Escaper)escaper1, (Escaper)escaper2 );
}

Escaper createEscaper(char *name, TechnionFaculty faculty, int skill, EscaperResult *result)
{
    Escaper newEscaper = NULL;

    if(name == NULL){
        *result = ESCAPER_INVALID_PARAMETER;
        return NULL;
    }
    if(skill < 1 || 10 < skill){
        *result = ESCAPER_INVALID_PARAMETER;
        return NULL;
    }

    if( checkEmail(name) != ESCAPER_SUCCESS){
        *result = ESCAPER_INVALID_PARAMETER;
        return NULL;
    }

    newEscaper = malloc(sizeof(struct SEscaper));
    if(newEscaper == NULL){
        *result = ESCAPER_OUT_OF_MEMORY;
        return NULL;
    }

    newEscaper->email = malloc( strlen(name) + 1 );
    if( newEscaper->email == NULL){
        free(newEscaper);
        *result = ESCAPER_OUT_OF_MEMORY;
        return NULL;
    }
    strcpy( newEscaper->email, name);
    newEscaper->skill_level = skill;
    newEscaper->faculty = faculty;

    *result = ESCAPER_SUCCESS;
    return newEscaper;
}

//checking for a valid email
EscaperResult static checkEmail(char *name)
{
    int counter = 0;
    int i = 0;
    while( name[i] != '\0' ){
        if( name[i] == '@' ){
            counter++;
        }
        i++;
    }
    if(counter == 0 || counter > 1){
        return ESCAPER_INVALID_PARAMETER;
    }

    return ESCAPER_SUCCESS;
}

char *escaperGetEmail(Escaper escaper, EscaperResult *result)
{
    char *email = NULL;

    if(escaper == NULL){
        *result = ESCAPER_NULL_PARAMETER;
        return NULL;
    }

    assert(escaper->email != NULL);
    email = malloc( strlen(escaper->email) + 1 );
    if(email == NULL){
        *result =  ESCAPER_OUT_OF_MEMORY;
        return NULL;
    }
    strcpy(email, escaper->email);
    *result =  ESCAPER_SUCCESS;

    return email;
}

int escaperGetSkill(Escaper escaper)
{
    return escaper->skill_level;
}

TechnionFaculty escaperGetFaculty(Escaper escaper)
{
    assert(escaper != NULL);
    return escaper->faculty;
}