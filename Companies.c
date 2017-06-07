#include "Companies.h"
#include "Rooms.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"



struct SCompany{
    char *email;
    TechnionFaculty faculty;
    Set rooms;
};

/**
 * Compares to companies to check if emails are identical
 * @param firstCompany : first company to compare
 * @param secondCompany : first company to compare
 * @return : 0 companies are identical |or| <0 or >0 if not identical (same as strcmp func)
 */
int compareCompanies(Company firstCompany , Company secondCompany)
{
    return strcmp(firstCompany->email , secondCompany->email));
}


void freeCompany(Company company)
{
    free(company->email);
    setDestroy(company->rooms);
    free(company);
}

Company copyCompany(Company company)
{
    Company newCompany=NULL;
    createCompany(company->email , company->faculty , newCompany);
    newCompany->rooms = setCopy(company->rooms);
    return newCompany;
}



CompanyResult createCompany(char *email, TechnionFaculty faculty, Company newCompany)
{
    if(email == NULL){
        return CMP_NULL_PARAMETER;
    }
    if( faculty < 1 || 10 < faculty){
        return CMP_INVALID_PARAMETER;
    }

    newCompany = malloc( sizeof(Company) );
    if(newCompany == NULL){
        return CMP_OUT_OF_MEMORY;
    }
    newCompany->email = malloc(strlen(email) + 1);
    if(newCompany->email == NULL){
        free(newCompany);
        return CMP_OUT_OF_MEMORY;
    }
    strcpy(newCompany->email, email);
    newCompany->faculty = faculty;

    copySetElements copyRoomPtr;
    copyRoomPtr = (SetElement)copyRoom;
    freeSetElements removeRoomPtr;
    removeRoomPtr = (SetElement)removeRoom;
    compareSetElements compareRoomPtr;
    compareRoomPtr = (SetElement)compareRoom;

    newCompany->rooms = setCreate(copyRoomPtr , removeRoomPtr , compareRoomPtr);

    return CMP_SUCCESS;
}
