#include "Companies.h"
#include "Rooms.h"
#include "set.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

static SetElement setCopyOfRoom(SetElement roomToCopy);
static void setRemoveRoom(SetElement room);
static int setCompareRoom(SetElement firstRoom, SetElement secondRoom);

struct SCompany{
    char *email;
    TechnionFaculty faculty;
    Set rooms;
};

static CompanyResult translateRoomResult(int roomResult)
{
    switch(roomResult){
        case ROOMS_INVALID_PARAMETER:
            return COMPANY_INVALID_PARAMETER;
        case ROOMS_OUT_OF_MEMORY:
            return COMPANY_OUT_OF_MEMORY;
        case ROOMS_NULL_PARAMETER:
            return COMPANY_NULL_PARAMETER;
        default:
            return COMPANY_SUCCESS;

    }
}

static CompanyResult translateSetResult(int setResult)
{
    switch(setResult){
        case SET_OUT_OF_MEMORY:
            return COMPANY_OUT_OF_MEMORY;
        case SET_NULL_ARGUMENT:
            return COMPANY_NULL_PARAMETER;
        case SET_ITEM_ALREADY_EXISTS:
            return COMPANY_ROOM_ID_ALREADY_EXIST;
        case SET_ITEM_DOES_NOT_EXIST:
            return COMPANY_ROOM_ID_DOES_NOT_EXIST;
        default:
            return COMPANY_SUCCESS;
    }
}

CompanyResult checkIfRoomExists(Company company , int id )
{
    //Room room = NULL;
    //roomResult createRoomResult;


    SET_FOREACH(Room, room, company->rooms){
        if(id == getRoomId(room) ){
            return COMPANY_ROOM_ID_ALREADY_EXIST;
        }
    }

    return COMPANY_ROOM_ID_DOES_NOT_EXIST;


    /*
    room = createRoom(company->email , id , 0 , 0 , 0 , 0 , 0 , &createRoomResult);
    if(translateRoomResult(createRoomResult) != COMPANY_SUCCESS){
        return translateRoomResult(createRoomResult);
    }
    if(setIsIn(company->rooms , room)){
        removeRoom(room);
        return COMPANY_ID_ALREADY_EXIST;
    }
    removeRoom(room);
    return COMPANY_ID_DOES_NOT_EXIST;
     */
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
    roomResult roomResult;
    Room newRoom = NULL;
    SetResult setResult;
    if(checkIfRoomExists(company, id) == COMPANY_ROOM_ID_ALREADY_EXIST){
        return COMPANY_ROOM_ID_ALREADY_EXIST;
    }
    newRoom = createRoom(company->email , id , price , num_ppl , start_time , finish_time , difficulty , &roomResult);
    if(translateRoomResult(roomResult) != COMPANY_SUCCESS){
        removeRoom(newRoom);
        return translateRoomResult(roomResult);
    }
    setResult = setAdd(company->rooms , newRoom);
    if(translateSetResult(setResult) != COMPANY_SUCCESS){
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
    return strcmp(firstCompany->email , secondCompany->email);
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
    CompanyResult companyResult;

    newCompany = createCompany(company->email , company->faculty , &companyResult);
    newCompany->rooms = setCopy(company->rooms);
    return newCompany;
}

Company createCompany(char *email, TechnionFaculty faculty, CompanyResult *result)
{
    Company newCompany;

    if(email == NULL){
        *result = COMPANY_NULL_PARAMETER;
        return NULL;
    }

    newCompany = malloc( sizeof(*newCompany) );
    if(newCompany == NULL){
        *result =  COMPANY_OUT_OF_MEMORY;
         return NULL;
    }
    newCompany->email = malloc(strlen(email) + 1);
    if(newCompany->email == NULL){
        free(newCompany);
        *result =  COMPANY_OUT_OF_MEMORY;
        return NULL;
    }
    strcpy(newCompany->email, email);
    newCompany->faculty = faculty;

    newCompany->rooms = setCreate(setCopyOfRoom , setRemoveRoom , setCompareRoom);
    if(newCompany->rooms == NULL){
        freeCompany(newCompany);
        *result = COMPANY_OUT_OF_MEMORY;
        return NULL;
    }

    *result =  COMPANY_SUCCESS;

    return newCompany;
}

CompanyResult addCompanyRoom(char *companyEmail,int roomId,int roomPrice,int num_ppl,int open_time,
                             int close_time, int diffuculty, Company company)
{
    SetResult setResult;
    roomResult roomResult;
    Room newRoom;

    if(company == NULL){
        return COMPANY_NULL_PARAMETER;
    }

    newRoom = createRoom(companyEmail, roomId, roomPrice, num_ppl, open_time, close_time, diffuculty, &roomResult);
    if(roomResult != ROOMS_SUCCESS){
        return translateRoomResult(roomResult);
    }
    assert(newRoom == NULL);

    setResult = setAdd(company->rooms, newRoom);
    if(setResult != SET_SUCCESS){
        removeRoom(newRoom);
        return translateSetResult(setResult);
    }

    return COMPANY_SUCCESS;
}

CompanyResult removeCompanyRoom(Company company, int roomId)
{
    SET_FOREACH(Room, room, company->rooms){
        if(roomId == getRoomId(room)){
            removeRoom(room);
            return COMPANY_SUCCESS;
        }
    }

    return COMPANY_ROOM_ID_DOES_NOT_EXIST;
}

static SetElement setCopyOfRoom(SetElement roomToCopy)
{
    return  (SetElement)copyRoom( (Room)roomToCopy );
}

static void setRemoveRoom(SetElement room)
{
    removeRoom( (Room)room );
}

static int setCompareRoom(SetElement firstRoom, SetElement secondRoom)
{
    return compareRoom(firstRoom, secondRoom);
}

CompanyResult companyRoomRemove(Company company, int roomId)
{
    if(company == NULL){
        return COMPANY_NULL_PARAMETER;
    }

    SET_FOREACH(Room, room, company->rooms){
        if(getRoomId(room) == roomId){
            setRemove(company->rooms, room);
            return COMPANY_SUCCESS;
        }
    }

    return COMPANY_ROOM_ID_DOES_NOT_EXIST;
}

char *getCompanyEmail(Company company, CompanyResult *result)
{
    char *email;
    email = malloc( strlen(company->email) + 1 );
    if(email == NULL){
        *result = COMPANY_OUT_OF_MEMORY;
        return NULL;
    }
    strcpy(email, company->email);

    *result = COMPANY_SUCCESS;

    return email;
}

CompanyResult getCompanyFaculty(Company company, TechnionFaculty *faculty)
{
    if(company == NULL){
        return COMPANY_NULL_PARAMETER;
    }

    *faculty = company->faculty;

    return COMPANY_SUCCESS;
}

int getCompanyRecommendedRoomId(Company company, int P_e, int skill_level, double *best_calculation)
    {
    double temp_calculation=0;
    Room best_room = NULL;

    SET_FOREACH(Room, room, company->rooms){
        temp_calculation = getRoomRecommendedCalculation(room, P_e, skill_level);
        if(*best_calculation > temp_calculation){
            best_room = room;
            *best_calculation = temp_calculation;
        }
    }

    return getRoomId(best_room);
}

int getCompanyRoomListSize(Company company)
{
    if(company == NULL){
        return -1;
    }
    assert(company->rooms != NULL);
    return setGetSize(company->rooms);
}