#include "Companies.h"
#include "EscapeTechnion.h"
#include "Rooms.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

    newCompany = malloc(sizeof(*newCompany));
    if(newCompany == NULL){
        return NULL;
    }
    newCompany->rooms = NULL;
    newCompany->rooms = setCopy(company->rooms);
    if(newCompany->rooms == NULL){
        free(newCompany);
        return NULL;
    }

    newCompany->email = malloc( strlen(company->email) + 1);
    if(newCompany->email == NULL){
        setDestroy(newCompany->rooms);
        free(newCompany);

        return NULL;
    }
    strcpy(newCompany->email, company->email);
    newCompany->faculty = company->faculty;

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

    newCompany->rooms = NULL;

    return CMP_SUCCESS;
}

CompanyResult getCompanyRooms(Company targetCompany , SetElement rooms)
{
    &rooms = targetCompany->&rooms;
    return CMP_SUCCESS;
}