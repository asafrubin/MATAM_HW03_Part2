#include "Escapers.h"
#include "set.h"
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <string.h>

EscaperResult static checkEmail(char *name);

struct SEscaper{
    char *email;
    TechnionFaculty faculty;
    int skill_level;
};

/**
 * Frees memory allocated to escaper element
 * @param escaper : element to free
 */
void freeEscaper(Escaper escaper)
{
    if(escaper) {
        free(escaper->email);
        escaper->email = NULL;
        free(escaper);
        escaper = NULL;
    }
}

/**
 * Aux function used by set.h
 * @param escaper
 */
void SetFreeEscaper(SetElement escaper)
{
    freeEscaper( (SetElement)escaper );
}

/**
 * Creates a copy of escaper element
 * @param escaper : element to copy
 * @return : copy of escaper element
 */
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

/**
 * Aux used by set.h
 * @param escaper
 * @return
 */
SetElement setEscaperCopyElement(SetElement escaper)
{
    return (SetElement)escaperCopyElement( (Escaper)escaper );
}

/**
 * Compares between to escaper elements
 * @param escaper1
 * @param escaper2
 * @return : '0' if identical other int else
 */
int escaperCompare(Escaper escaper1, Escaper escaper2)
{
    assert(escaper1 != NULL);
    assert(escaper2 != NULL);
    return strcmp( escaper1->email, escaper2->email);
}

/**
 * Aux used by set.h
 * @param escaper
 * @return
 */
int SetEscaperCompare(SetElement escaper1, SetElement escaper2)
{
    return escaperCompare( (Escaper)escaper1, (Escaper)escaper2 );
}

/**
 * Creates a new escaper element
 * @param name : email of new escaper
 * @param faculty : faculty to which the escaper belongs
 * @param skill : escaper skill level
 * @param result : EscaperResult (ENUM)
 * @return : newly created escaper
 */
Escaper createEscaper(char *name, TechnionFaculty faculty, int skill, EscaperResult *result)
{
    Escaper newEscaper = NULL;

    if(name == NULL){
        *result = ESCAPER_INVALID_PARAMETER;
        return NULL;
    }
    if(skill < 1 || 10 < skill || faculty == UNKNOWN){
        *result = ESCAPER_INVALID_PARAMETER;
        return NULL;
    }

    if( checkEmail(name) != ESCAPER_SUCCESS){
        *result = ESCAPER_INVALID_PARAMETER;
        return NULL;
    }

    newEscaper = malloc(sizeof(*newEscaper));
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

/**
 * Checks if email is valid according to specific rules
 * @param name : email
 * @return roomResult (ENUM)
 */
EscaperResult static checkEmail(char *name)
{
    if(name == NULL){
        return ESCAPER_INVALID_PARAMETER;
    }

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

/**
 * Retrieves an escapers email
 * @param escaper : escaper which is inquired about
 * @param result : EscaperResult (ENUM)
 * @return : string which hilds escapers email
 */
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

/**
 * Retrieves an escapers skill level
 * @param escaper : escaper which is inquired about
 * @param result : EscaperResult (ENUM)
 * @return : skill level of escaper
 */
int escaperGetSkill(Escaper escaper)
{
    return escaper->skill_level;
}

/**
 * Retrieves an escapers faculty
 * @param escaper : escaper which is inquired about
 * @param result : EscaperResult (ENUM)
 * @return : faculty of escaper
 */
TechnionFaculty escaperGetFaculty(Escaper escaper)
{
    assert(escaper != NULL);
    return escaper->faculty;
}