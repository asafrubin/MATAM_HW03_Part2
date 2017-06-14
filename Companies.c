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

/**
 * Translates roomResult ENUM to CompanyResult ENUM
 * @param roomResult : ENUM to translate
 * @return : CompanyResult ENUM
 */
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

/**
 * Translates setResult ENUM to CompanyResult ENUM
 * @param roomResult : ENUM to translate
 * @return : CompanyResult ENUM
 */
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

/**
 * Checks if email is valid according to specific rules
 * @param name : email
 * @return : roomResult (ENUM)
 */
CompanyResult static checkEmail(char *name)
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
        return COMPANY_INVALID_PARAMETER;
    }

    return COMPANY_SUCCESS;
}

/**
 * Checks if a room exists within a company by it's id
 * @param company : company struct
 * @param id : room to check
 * @return : roomResult (ENUM)
 */
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
 * @return : roomResult (ENUM)
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

/**
 * Aux function used for set.h
 * @param firstCompany
 * @param secondCompany
 * @return
 */
int setCompareCompanies(SetElement firstCompany, SetElement secondCompany)
{
    return compareCompanies( (Company)firstCompany, (Company)secondCompany );
}

/**
 * Frees all memory alloced for a company, including all rooms
 * @param company : struct to free
 */
void freeCompany(Company company)
{
    if(company) {
        free(company->email);
        setDestroy(company->rooms);
        free(company);
    }
}

/**
 * Aux function used for set.h
 * @param company
 */
void setFreeCompany(SetElement company)
{
    freeCompany( (Company)company );
}

/**
 * Creates an identical company as input
 * @param company : company to copy
 * @return : Element of copy of company
 */
Company copyCompany(Company company)
{
    Company newCompany = NULL;
    CompanyResult companyResult;

    newCompany = createCompany(company->email , company->faculty , &companyResult);
    if(newCompany == NULL){
        return NULL;
    }
    newCompany->rooms = setCopy(company->rooms);
    if(newCompany->rooms == NULL){
        free(newCompany);
        return NULL;
    }

    return newCompany;
}

/**
 * Aux function used for set.h
 * @param company
 * @return
 */
SetElement setCopyCompany(SetElement company)
{
    return (SetElement)copyCompany( (Company)company );
}

/**
 * Creates a new company
 * @param email : email of company
 * @param faculty : faculty to which the company belongs
 * @param result : roomResult (ENUM)
 * @return : Element of newly created company
 */
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
    *result = checkEmail(email);
    if(*result != COMPANY_SUCCESS){
        return NULL;
    }
    if(faculty > 18 || faculty < 0){
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

/**
 * Adds a new room to an existing company
 * @param companyEmail : company email of targeted company
 * @param roomId : unique id of new room
 * @param roomPrice : price of new room
 * @param num_ppl : number of people recommend for room
 * @param open_time : time room opens
 * @param close_time : time room closes
 * @param difficulty : difficulty of room
 * @param company : struct of company
 * @return : roomResult (ENUM)
 */
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

    return COMPANY_SUCCESS;
}

/**
 * Checks that following parameters are valid inputs to create a new room
 * @param companyEmail : company email of targeted company
 * @param roomId : unique id of new room
 * @param roomPrice : price of new room
 * @param num_ppl : number of people recommend for room
 * @param open_time : time room opens
 * @param close_time : time room closes
 * @param difficulty : difficulty of room
 * @return : roomResult (ENUM)
 */
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

/**
 * Removes a room from a company by room id
 * @param company : company from which to remove
 * @param roomId : unique id of room to remove
 * @return : roomResult (ENUM)
 */
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

/**
 * Checks if room is open during requested hour
 * @param company : compnay to which room belongs
 * @param roomId : unique id of room
 * @param req_hour : hour to inquire about
 * @return : roomResult (ENUM)
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

/**
 * Aux function used for set.h
 * @param roomToCopy
 * @return
 */
static SetElement setCopyOfRoom(SetElement roomToCopy)
{
    return  (SetElement)copyRoom( (Room)roomToCopy );
}

/**
 * Aux function used for set.h
 * @param room
 */
static void setRemoveRoom(SetElement room)
{
    removeRoom( (Room)room );
}

/**
 * Aux function used for set.h
 * @param firstRoom
 * @param secondRoom
 * @return
 */
static int setCompareRoom(SetElement firstRoom, SetElement secondRoom)
{
    return compareRoom(firstRoom, secondRoom);
}

/**
 * Removes a specific room withing a given company
 * @param company : company to which the room belongs
 * @param roomId : unique id of room
 * @return : roomResult (ENUM)
 */
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

/**
 * Retrieves company email
 * @param company : company inquered about
 * @param result : roomResult (ENUM)
 * @return : string holding company email
 */
char *getCompanyEmail(Company company, CompanyResult *result)
{
    char *email;
    assert(company != NULL);
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

/**
 * Retrieves company faculty
 * @param company : company inquered about
 * @param faculty : faculty to which the company belongs
 * @return : roomResult (ENUM)
 */
CompanyResult getCompanyFaculty(Company company, TechnionFaculty *faculty)
{
    if(company == NULL){
        return COMPANY_NULL_PARAMETER;
    }

    *faculty = company->faculty;

    return COMPANY_SUCCESS;
}

/**
 * Retrieves room most recommend ID
 * @param company : company withing which to inquire
 * @param P_e : number of people which order the room
 * @param skill_level : skill level of order
 * @param best_calculation : pointer which holds the calculation
 * @return : recommended room id
 */
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

/**
 * Retrieves number of rooms within a company
 * @param company : company which to inquire
 * @return : number of rooms within the company
 */
int getCompanyRoomListSize(Company company)
{
    if(company == NULL){
        return -1;
    }
    assert(company->rooms != NULL);
    return setGetSize(company->rooms);
}

/**
 * Retrieves room price by id
 * @param company : company to which the room belongs to
 * @param roomId : unique id of room
 * @return : price of room inquired about
 */
int getCompanyRoomPriceById(Company company, int roomId)
{
    SET_FOREACH(Room, room, company->rooms){
        if( getRoomId(room) == roomId){
            return getRoomPrice(room);
        }
    }

    return -1;
}

/**
 * Retrieves room difficulty by id
 * @param company : company to which the room belongs
 * @param roomId : unique id of room
 * @return : difficulty of room
 */
int getCompanyRoomDifficultyById(Company company, int roomId)
{
    SET_FOREACH(Room, room, company->rooms){
        if( getRoomId(room) == roomId){
            return getRoomDifficulty(room);
        }
    }

    return -1;
}

/**
 * Retrieves number of people recommended for room
 * @param company : company to which the room belongs
 * @param roomId : unique id of room
 * @return : number of people recommended for room
 */
int getCompanyRoomNumOfPplById(Company company, int roomId)
{
    SET_FOREACH(Room, room, company->rooms){
        if( getRoomId(room) == roomId){
            return getRoomNumOfPpl(room);
        }
    }

    return -1;
}