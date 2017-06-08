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

CompanyResult translateRoomResult(int roomResult)
{
    switch(roomResult){
        case ROOMS_OUT_OF_MEMORY:
            return COMPANY_OUT_OF_MEMORY;
        case ROOMS_NULL_PARAMETER:
            return COMPANY_NULL_PARAMETER;
        default:
            return COMPANY_SUCCESS;

    }
}

CompanyResult translateSetResult(int setResult)
{
    switch(setResult){
        case SET_OUT_OF_MEMORY:
            return COMPANY_OUT_OF_MEMORY;
        case SET_NULL_ARGUMENT:
            return COMPANY_NULL_PARAMETER;
        case SET_ITEM_ALREADY_EXISTS:
            return ROOMS_ID_ALREADY_EXIST;
        case SET_ITEM_DOES_NOT_EXIST:
            return ROOMS_ID_DOES_NOT_EXIST;
        default:
            return COMPANY_SUCCESS;
    }
}

CompanyResult checkIfRoomExists(Company company , int id )
{
    Room room = NULL;

    roomResult createRoomResult = createRoom(company->email , id , NULL , NULL , NULL , NULL ,
                                             NULL , room);
    if(translateRoomResult(createRoomResult) != COMPANY_SUCCESS){
        return translateRoomResult(createRoomResult);
    }
    if(setIsIn(company->rooms , room)){
        removeRoom(room);
        return ROOMS_ID_ALREADY_EXIST;
    }
    removeRoom(room);
    return ROOMS_ID_DOES_NOT_EXIST;
}


/**
 * Adds a new room to a Companies Set
 * @param company : the company to which to add the room
 * @param id : unique di for room
 * @param price : price for room
 * @param num_ppl :  num of people for room
 * @param start_time :  opening time
 * @param finish_time : closing time
 * @param difficulty : room difficulty
 * @return
 */
CompanyResult addRoomToCompany(Company company , int id , int price , int num_ppl , int start_time , int
                                finish_time , int difficulty)
{
    int roomResult;
    Room newRoom = NULL;
    if(checkIfRoomExists(company , id) != ROOMS_SUCCESS){
        removeRoom(newRoom);
        return checkIfRoomExists(company , id);
    }
    roomResult = createRoom(company->email , id , price , num_ppl , start_time , finish_time , difficulty , newRoom);
    if(translateRoomResult(roomResult) != COMPANY_SUCCESS){
        removeRoom(newRoom);
        return translateRoomResult(roomResult);
    }
    roomResult = setAdd(company->rooms , newRoom);
    if(translateRoomResult(roomResult) != COMPANY_SUCCESS){
        removeRoom(newRoom);
        return translateRoomResult(roomResult);
    }
    removeRoom(newRoom);
    return COMPANY_SUCCESS;
}

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
    Company newCompany = NULL;
    createCompany(company->email , company->faculty , newCompany);
    newCompany->rooms = setCopy(company->rooms);
    return newCompany;
}



CompanyResult createCompany(char *email, TechnionFaculty faculty, Company newCompany)
{
    if(email == NULL){
        return COMPANY_NULL_PARAMETER;
    }
    if( faculty < 1 || 10 < faculty){
        return COMPANY_INVALID_PARAMETER;
    }

    newCompany = malloc( sizeof(Company) );
    if(newCompany == NULL){
        return COMPANY_OUT_OF_MEMORY;
    }
    newCompany->email = malloc(strlen(email) + 1);
    if(newCompany->email == NULL){
        free(newCompany);
        return COMPANY_OUT_OF_MEMORY;
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

    return COMPANY_SUCCESS;
}