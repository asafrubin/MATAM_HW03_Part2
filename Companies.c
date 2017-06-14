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
        case ROOMS_ROOM_NOT_AVAILABLE:
            return COMPANY_NO_ROOMS_AVAILABLE;
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

    assert(company != NULL);

    SET_FOREACH(Room, room, company->rooms){
        if(id == getRoomId(room) ){
            return COMPANY_ROOM_ID_ALREADY_EXIST;
        }
    }

    return COMPANY_ROOM_ID_DOES_NOT_EXIST;
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
    assert(firstCompany != NULL);
    assert(secondCompany != NULL);
    return strcmp(firstCompany->email , secondCompany->email);
}

int setCompareCompanies(SetElement firstCompany, SetElement secondCompany)
{
    return compareCompanies( (Company)firstCompany, (Company)secondCompany );
}

void freeCompany(Company company)
{
    if(company) {
        free(company->email);
        setDestroy(company->rooms);
        free(company);
    }
}

void setFreeCompany(SetElement company)
{
    freeCompany( (Company)company );
}

Company copyCompany(Company company)
{
    Company newCompany = NULL;
    CompanyResult companyResult;

    newCompany = createCompany(company->email, company->faculty, &companyResult);
    if(newCompany == NULL){
        return NULL;
    }
    setDestroy(newCompany->rooms);
    newCompany->rooms = setCopy(company->rooms);
    if(newCompany->rooms == NULL){
        free(newCompany);
        return NULL;
    }

    return newCompany;
}

SetElement setCopyCompany(SetElement company)
{
    return (SetElement)copyCompany( (Company)company );
}

Company createCompany(char *email, TechnionFaculty faculty, CompanyResult *result)
{
    Company newCompany;

    if(email == NULL){
        *result = COMPANY_INVALID_PARAMETER;
        return NULL;
    }
    if(faculty == UNKNOWN){
        *result = COMPANY_INVALID_PARAMETER;
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

static void setRemoveRoom(SetElement room)
{
    removeRoom( (Room)room );
}

CompanyResult addCompanyRoom(char *companyEmail,int roomId,int roomPrice,int num_ppl,int open_time,
                             int close_time, int difficulty, Company company)
{
    SetResult setResult;
    roomResult roomResult;
    Room newRoom;

    if(company == NULL){
        return COMPANY_NULL_PARAMETER;
    }

    newRoom = createRoom(companyEmail, roomId, roomPrice, num_ppl, open_time, close_time, difficulty, &roomResult);
    if(roomResult != ROOMS_SUCCESS){
        return translateRoomResult(roomResult);
    }
    assert(newRoom != NULL);

    setResult = setAdd(company->rooms, newRoom);
    if(setResult != SET_SUCCESS){
        removeRoom(newRoom);
        return translateSetResult(setResult);
    }
    removeRoom(newRoom);

    return COMPANY_SUCCESS;
}
CompanyResult mtmCheckCompanyRoomParameters(char *companyEmail, int roomId, int roomPrice, int num_ppl, int open_time,
                                            int close_time, int difficulty)
{
    roomResult roomResult;

    if(companyEmail == NULL){
        return COMPANY_INVALID_PARAMETER;
    }
    roomResult = checkRoomParameters(companyEmail, roomId, roomPrice, num_ppl, open_time, close_time, difficulty);
    if(roomResult != ROOMS_SUCCESS){
        return COMPANY_INVALID_PARAMETER;
    }

    return COMPANY_SUCCESS;
}

/*
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
 */

CompanyResult checkIfCompanyRoomIsOpenById(Company company, int roomId, int req_hour)
{
    int tempID;
    roomResult roomResult;
    SET_FOREACH(Room, room, company->rooms){
       tempID =  getRoomId(room);
        if(tempID == roomId){
            roomResult = roomCheckIfOpen(room, req_hour);
            return translateRoomResult(roomResult);
            }
        }

    return COMPANY_ROOM_ID_DOES_NOT_EXIST;
}

static SetElement setCopyOfRoom(SetElement roomToCopy)
{
    return  (SetElement)copyRoom( (Room)roomToCopy );
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
    assert(company != NULL);

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

static int calculateFacultyDistanceFromClient(TechnionFaculty escaperFaculty, TechnionFaculty companyFaculty,
                                              TechnionFaculty tempCompanyFaculty, int tempId, int id,
                                              CompanyResult *companyResult)
{
    int distance1, distance2;

    distance1 = abs(companyFaculty - escaperFaculty);
    distance2 = abs(tempCompanyFaculty - escaperFaculty);

    if ((distance1 - distance2) == 0) {
        if (companyFaculty == tempCompanyFaculty) {
            *companyResult = COMPANY_CALC_BY_ID;
            if(tempId < id){
                return tempId;
            }
            else
                return id;
        }
        if(companyFaculty < tempCompanyFaculty){
            *companyResult = COMPANY_CALC_BY_FACULTY;
            return companyFaculty;
        }
        else
            *companyResult = COMPANY_CALC_BY_FACULTY;
        return tempCompanyFaculty;
    }
    else if(distance1 < distance2 ){
        *companyResult = COMPANY_CALC_BY_FACULTY;
        return companyFaculty;
    }
    else
        *companyResult = COMPANY_CALC_BY_FACULTY;
    return tempCompanyFaculty;
}

int getCompanyRecommendedRoomId(Company company, int P_e, int skill_level, int *best_calculation,
                                TechnionFaculty escaperFaculty, TechnionFaculty *companyFaculty,
                                TechnionFaculty *tempCompanyFaculty, int *roomId, int *roomPrice)
{
    int temp_calculation = 0, best_room = 0, tempId=0, calcResult=0;
    CompanyResult companyResult;

    SET_FOREACH(Room, room, company->rooms) {
        temp_calculation = getRoomRecommendedCalculation(room, P_e, skill_level);
        if (*best_calculation > temp_calculation) {
            *roomId = getRoomId(room);
            *best_calculation = temp_calculation;
            *companyFaculty = *tempCompanyFaculty;
            *roomPrice = getCompanyRoomPriceById(company, *roomId);
        } else if (*best_calculation == temp_calculation) {
            calcResult = calculateFacultyDistanceFromClient(escaperFaculty, *companyFaculty,
                                                            *tempCompanyFaculty, tempId, *roomId, &companyResult);
            if (companyResult == COMPANY_CALC_BY_FACULTY) {
                *companyFaculty = (TechnionFaculty) companyResult;
                if (*companyFaculty == *tempCompanyFaculty) {
                    *roomId = tempId;
                    *roomPrice = getCompanyRoomPriceById(company, *roomId);
                }
            } else {
                *roomId = calcResult;
                if (*roomId == tempId) {
                    *companyFaculty = *tempCompanyFaculty;
                    *roomPrice = getCompanyRoomPriceById(company, *roomId);
                }
            }
        }
    }

    return best_room;
}



int getCompanyRoomListSize(Company company)
{
    if(company == NULL){
        return -1;
    }
    assert(company->rooms != NULL);
    return setGetSize(company->rooms);
}

int getCompanyRoomPriceById(Company company, int roomId)
{
    SET_FOREACH(Room, room, company->rooms){
        if( getRoomId(room) == roomId){
            return getRoomPrice(room);
        }
    }

    return -1;
}

int getCompanyRoomDifficultyById(Company company, int roomId)
{
    SET_FOREACH(Room, room, company->rooms){
        if( getRoomId(room) == roomId){
            return getRoomDifficulty(room);
        }
    }

    return -1;
}

int getCompanyRoomNumOfPplById(Company company, int roomId)
{
    SET_FOREACH(Room, room, company->rooms){
        if( getRoomNumOfPpl(room) == roomId){
            return getRoomPrice(room);
        }
    }

    return -1;
}