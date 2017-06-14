#include "EscapeTechnion.h"
#include "set.h"
#include "list.h"
#include "Companies.h"
#include "Escapers.h"
#include "Orders.h"
#include "test_utilities.h"
#include <limits.h>
#include <mem.h>
#include <stdlib.h>
#include <float.h>

#define MAX_DAYS 365
#define MAX_HOUR 23

struct SEscapeTechnion{
    FILE *outputStream;
    int *total_revenue;
    int day;
    Set escapers;
    Set companies;
    List orders;
};

/**
 * Creates a new data type of EscapeTechnion
 * @param output : where to print to
 * @return : EscapeTechnion (ENUM)
 */
EscapeTechnion mtmCreateEscapeTechnion(FILE *output)
{
    EscapeTechnion escapeTechnion;
    escapeTechnion = malloc(sizeof(*escapeTechnion));
    if(escapeTechnion == NULL){
        return NULL;
    }
    //init total_revenue
    escapeTechnion->total_revenue = malloc( UNKNOWN * sizeof(int) );
    if(escapeTechnion->total_revenue == NULL){
        free(escapeTechnion);
        return NULL;
    }
    for (int i = 0; i < UNKNOWN; ++i) {
        escapeTechnion->total_revenue[i] = -1;
    }
    //init escapers
    escapeTechnion->escapers = setCreate(setEscaperCopyElement, SetFreeEscaper, SetEscaperCompare);
    if(escapeTechnion->escapers == NULL){
        free(escapeTechnion);
        free(escapeTechnion->total_revenue);
        return NULL;
    }
    //init companies
    escapeTechnion->companies = setCreate(setCopyCompany, setFreeCompany, setCompareCompanies);
    if(escapeTechnion->companies == NULL){
        free(escapeTechnion);
        free(escapeTechnion->total_revenue);
        setDestroy(escapeTechnion->escapers);
        return NULL;
    }
    //init orders
    escapeTechnion->orders = listCreate(listCopyOrder, listFreeOrder);
    if(escapeTechnion->orders == NULL){
        free(escapeTechnion);
        free(escapeTechnion->total_revenue);
        setDestroy(escapeTechnion->escapers);
        setDestroy(escapeTechnion->companies);
        return NULL;
    }
    //init day and output stream
    escapeTechnion->day = 0;
    escapeTechnion->outputStream = output;

    return escapeTechnion;
}

/**
 * Removes data type EscapeTechnion
 * @param escapeTechnion : Element to be removed
 */
void mtmDestroyEscapeTechnion(EscapeTechnion escapeTechnion)
{
    if(escapeTechnion) {
        setDestroy(escapeTechnion->companies);
        setDestroy(escapeTechnion->escapers);
        listDestroy(escapeTechnion->orders);
        free(escapeTechnion->total_revenue);
    }
}

/**
 * Translates EscaperResult to MtmErrorCode
 * @param escaperResult : to be translated
 * @return : translated
 */
static MtmErrorCode translateEscaperResultToMtm(EscaperResult escaperResult)
{
    switch(escaperResult){
        case ESCAPER_OUT_OF_MEMORY:
            return MTM_OUT_OF_MEMORY;
        case ESCAPER_NULL_PARAMETER:
            return MTM_NULL_PARAMETER;
        case ESCAPER_INVALID_PARAMETER:
            return MTM_INVALID_PARAMETER;
        default:
            return MTM_SUCCESS;
    }
}

/**
 * Translates CompanyResult to MtmErrorCode
 * @param escaperResult : to be translated
 * @return : translated
 */
static MtmErrorCode translateCompanyResultToMtm(CompanyResult companyResult)
{
    switch(companyResult){
        case COMPANY_INVALID_PARAMETER:
            return MTM_INVALID_PARAMETER;
        case COMPANY_OUT_OF_MEMORY:
            return MTM_OUT_OF_MEMORY;
        case COMPANY_NULL_PARAMETER:
            return MTM_NULL_PARAMETER;
        default:
            return MTM_SUCCESS;

    }

}

/**
 * Translates SetResult to MtmErrorCode
 * @param escaperResult : to be translated
 * @return : translated
 */
static MtmErrorCode translateSetResultToMtm(SetResult setResult)
{
    switch(setResult){
        case SET_OUT_OF_MEMORY:
            return MTM_OUT_OF_MEMORY;
        case SET_NULL_ARGUMENT:
            return MTM_NULL_PARAMETER;
        case SET_ITEM_ALREADY_EXISTS:
            return MTM_ID_ALREADY_EXIST;
        case SET_ITEM_DOES_NOT_EXIST:
            return MTM_ID_DOES_NOT_EXIST;
        default:
            return MTM_SUCCESS;
    }
}

/**
 * Translates orderResult to MtmErrorCode
 * @param escaperResult : to be translated
 * @return : translated
 */
static MtmErrorCode translateOrderResultToMtm(orderResult orderResult)
{
    switch(orderResult){
        case ORDER_OUT_OF_MEMORY:
            return MTM_OUT_OF_MEMORY;
        case ORDER_NULL_PARAMETER:
            return MTM_NULL_PARAMETER;
        case ORDER_ID_ALREADY_EXIST:
            return MTM_ID_ALREADY_EXIST;
        case ORDER_ID_DOES_NOT_EXIST:
            return MTM_ID_DOES_NOT_EXIST;
        default:
            return MTM_SUCCESS;
    }
}

/**
 * Translates ListResult to MtmErrorCode
 * @param escaperResult : to be translated
 * @return : translated
 */
static MtmErrorCode translateListResultToMtm(ListResult listResult)
{
    switch(listResult){
        case LIST_OUT_OF_MEMORY:
            return MTM_OUT_OF_MEMORY;
        case LIST_NULL_ARGUMENT:
            return MTM_NULL_PARAMETER;
        case LIST_INVALID_CURRENT:
            return MTM_INVALID_PARAMETER;
        default:
            return MTM_SUCCESS;
    }
}

/**
 * Retrieves price of room
 * @param companies : set of companies within the system
 * @param companyFaculty : faculty which owns companies
 * @param roomId : unique id of room
 * @return : price of inquired room
 */
static int mtmGetRoomPriceByFacultyAndId(Set companies,TechnionFaculty companyFaculty, int roomId)
{
    TechnionFaculty tempFaculty;

    SET_FOREACH(Company, company, companies){
        getCompanyFaculty(company, &tempFaculty);
        if(tempFaculty == companyFaculty){
           return getCompanyRoomPriceById(company, roomId);
        }
    }

    return -1;
}

/**
 * Retrieves revenue made by all faculties
 * @param escapeTechnion : system
 * @return : total revenue
 */
static int mtmGetTotalRevenue(EscapeTechnion escapeTechnion)
{
    int total = 0;

    assert(escapeTechnion != NULL);
    for (int i = 0; i < UNKNOWN; ++i) {
        if(escapeTechnion->total_revenue[i] == -1){
            continue;
        }
        total += escapeTechnion->total_revenue[i];
    }

    return total;
}

/*
static int mtmGetRoomNumOfPplByFacultyAndId(Set companies, TechnionFaculty companyFaculty, int roomId)
{
    TechnionFaculty tempFaculty;

    SET_FOREACH(Company, company, companies){
        getCompanyFaculty(company, &tempFaculty);
        if(tempFaculty == companyFaculty){
            return getCompanyRoomNumOfPplById(company, roomId);
        }
    }

    return -1;
}
 */

/**
 * Retrieves difficulty of room
 * @param companies : set of companies within the system
 * @param companyFaculty : faculty which owns companies
 * @param roomId : unique id of room
 * @return : difficulty of inquired room
 */
static int mtmGetRoomDifficultyByFacultyAndId(Set companies, int roomFaculty, int roomId)
{
    TechnionFaculty tempFaculty;

    SET_FOREACH(Company, company, companies){
        getCompanyFaculty(company, &tempFaculty);
        if(tempFaculty == roomFaculty){
            return getCompanyRoomDifficultyById(company, roomId);
        }
    }

    return -1;

}

/**
 * Retrives escaper skill
 * @param escapeTechnion : system
 * @param escaperEmail : unique email of escaper
 * @return : skill of escaper
 */
static int mtmGetEscaperSkillByEmail(EscapeTechnion escapeTechnion, char *escaperEmail)
{
    char *tempEmail = NULL;
    EscaperResult escaperResult;

    SET_FOREACH(Escaper, escaper, escapeTechnion->escapers){
        tempEmail = escaperGetEmail(escaper, &escaperResult);
        if( strcmp(tempEmail, escaperEmail) == 0 ){
            free(tempEmail);
            return escaperGetSkill(escaper);
        }
    }

    free(tempEmail);

    return -1;
}

/**
 * Retrives company by email
 * @param email : unique email of conmpany
 * @param escapeTechnion : system
 * @param mtmResult : ENUM
 * @return : company requestes
 */
static Company mtmGetCompanyByEmail(char *email, EscapeTechnion escapeTechnion, MtmErrorCode *mtmResult)
{
    char *companyEmail = NULL;
    CompanyResult companyResult;

    SET_FOREACH(Company, company, escapeTechnion->companies){
        companyEmail = getCompanyEmail(company, &companyResult);
        if(companyEmail == NULL){
            *mtmResult = translateCompanyResultToMtm(companyResult);
            return NULL;
        }
        assert(companyEmail != NULL);
        if(strcmp(email, companyEmail) == 0){
            free(companyEmail);
            *mtmResult = translateCompanyResultToMtm(companyResult);

            return company;
        }
    }

    free(companyEmail);
    *mtmResult = MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
    return NULL;
}

/**
 * Inner function used to find faculty closest to a specific clients faculty
 * @param escaperFaculty : faculty of client
 * @param companyFaculty : current
 * @param tempCompanyFaculty
 * @param tempId
 * @param id
 * @param companyResult
 * @return
 */
static int calculateFacultyDistanceFromClient(TechnionFaculty escaperFaculty, TechnionFaculty companyFaculty,
                                              TechnionFaculty tempCompanyFaculty, int tempId, int id,
                                              CompanyResult *companyResult){
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
        //else
            *companyResult = COMPANY_CALC_BY_FACULTY;
            return tempCompanyFaculty;
    }
    else if(distance1 < distance2 ){
        *companyResult = COMPANY_CALC_BY_FACULTY;
        return companyFaculty;
    }
    //else
        *companyResult = COMPANY_CALC_BY_FACULTY;
        return tempCompanyFaculty;
}

/**
 * Checks if company email already exists
 * @param escapeTechnion : system
 * @param email : email to check
 * @return : ENUM
 */
static MtmErrorCode mtmCheckIfCompanyEmailExists(EscapeTechnion escapeTechnion, char *email)
{
    char *companyEmail = NULL;
    CompanyResult companyResult;

    SET_FOREACH(Company, company, escapeTechnion->companies){
        companyEmail = getCompanyEmail(company, &companyResult);
        if(companyEmail == NULL){
            return translateCompanyResultToMtm(companyResult);
        }
        assert(companyEmail != NULL);
        if(strcmp(email, companyEmail) == 0){
            free(companyEmail);

            return MTM_EMAIL_ALREADY_EXISTS;
        }
    }

    free(companyEmail);
    return MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
}

/**
 * Retrieves an email of a comapny which belongs to a faculty
 * @param Companies : set of companies in system
 * @param companyFaculty : faculty requested
 * @return ; email of company
 */
static char *mtmGetCompanyEmailByFaculty(Set Companies, TechnionFaculty companyFaculty)
{
    char *email = NULL;
    CompanyResult companyResult;
    TechnionFaculty tempFaculty;

    SET_FOREACH(Company, company, Companies){
        getCompanyFaculty(company, &tempFaculty );
        if( companyFaculty == tempFaculty ){
            email = getCompanyEmail(company, &companyResult);
            if(companyResult != COMPANY_SUCCESS){
                return NULL;
            }
            return email;
        }
    }
    return NULL;
}

/**
 *
 * @param companies
 * @param email
 * @param mtmErrorCode
 * @return
 */
static TechnionFaculty mtmGetCompanyFacultyByEmail(Set companies, char *email, MtmErrorCode *mtmErrorCode)
{
    char *tempEmail;
    CompanyResult companyResult;
    TechnionFaculty faculty;

    SET_FOREACH(Company, company, companies){
        tempEmail = getCompanyEmail(company, &companyResult);
        assert(tempEmail != NULL);
        if(strcmp(tempEmail, email) == 0 ){
            companyResult = getCompanyFaculty(company, &faculty);
            assert(companyResult == COMPANY_SUCCESS);
            *mtmErrorCode = MTM_SUCCESS;
            return faculty;
        }
    }

    *mtmErrorCode = MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
    return UNKNOWN;
}

/**
 * Retrives escaper faculty
 * @param escapeTechnion : system
 * @param escaperEmail : unique email of escaper
 * @return : faculty of escaper
 */
static MtmErrorCode mtmGetEscaperFacultyByEmail(Set escapers, char *escaperEmail, TechnionFaculty *faculty)
{
    char *tempEmail = NULL;
    EscaperResult escaperResult;

    SET_FOREACH(Escaper, escaper, escapers){
        tempEmail = escaperGetEmail(escaper, &escaperResult);
        if(tempEmail == NULL){
            return MTM_OUT_OF_MEMORY;
        }
        if( strcmp(tempEmail, escaperEmail) == 0){
            free(tempEmail);
            *faculty = escaperGetFaculty(escaper);

            return MTM_SUCCESS;
        }

    }
    free(tempEmail);

    return MTM_NULL_PARAMETER;

}

/**
 * Checks if pending orders exist for a room
 * @param orders : list in system
 * @param desiredFaculty : faculty to which the room belongs
 * @param id : unique id of room
 * @return : ENUM
 */
static MtmErrorCode checkIfOrderExistForThisRoom(List orders, TechnionFaculty desiredFaculty, int id)
{
    TechnionFaculty tempFaculty;
    int tempId;

    LIST_FOREACH(Order, order, orders){
        tempId = getOrderRoomId(order);
        tempFaculty = getOrderRoomFaculty(order);
        if(tempId == id && tempFaculty == desiredFaculty){
            return MTM_RESERVATION_EXISTS;
        }
    }

    return MTM_SUCCESS;
}

/**
 * Checks if there are pending orders for a specific company
 * @param escapeTechnion : system
 * @param company : company inquired about
 * @return : ENUM
 */
static MtmErrorCode checkIfOrderExistForThisCompany(EscapeTechnion escapeTechnion, Company company)
{
    assert(escapeTechnion != NULL);
    assert(company != NULL);
    TechnionFaculty faculty, tempFaculty;

    getCompanyFaculty(company, &faculty);
    assert(faculty >= 0 && faculty < 18);
    LIST_FOREACH(Order, order, escapeTechnion->orders){
        tempFaculty = getOrderRoomFaculty(order);
        if(tempFaculty == faculty){
            return MTM_RESERVATION_EXISTS;
        }
    }

    return MTM_SUCCESS;
}

/**
 * Checks if email is valid according to specific rules
 * @param name : email
 * @return roomResult (ENUM)
 */
static MtmErrorCode checkEmail(char *name)
{
    if(name == NULL){
        return MTM_INVALID_PARAMETER;
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
        return MTM_INVALID_PARAMETER;
    }

    return MTM_SUCCESS;
}

/**
 * Checks if escaper exists within the system
 * @param escapeTechnion : system
 * @param escaperEmail : unique email of escaper
 * @return : ENUM
 */
static MtmErrorCode checkIfEscaperExist(EscapeTechnion escapeTechnion, char *escaperEmail)
{
    char *tempEmail = NULL;
    EscaperResult escaperResult;

    SET_FOREACH(Escaper, escaper, escapeTechnion->escapers){
        tempEmail = escaperGetEmail(escaper, &escaperResult);
        if(escaperResult != ESCAPER_SUCCESS){
            return MTM_OUT_OF_MEMORY;
        }
        assert(tempEmail != NULL);
        if( strcmp(tempEmail, escaperEmail) == 0 ){
            free(tempEmail);
            return MTM_SUCCESS;
        }
    }

    free(tempEmail);

    return MTM_CLIENT_EMAIL_DOES_NOT_EXIST;
}

/**
 * Checks if email exists within the system
 * @param escapeTechnion : system
 * @param email : email inquered about
 * @return : ENUM
 */
static MtmErrorCode mtmCheckIfEmailExists(EscapeTechnion escapeTechnion, char *email)
{
    if( mtmCheckIfCompanyEmailExists(escapeTechnion, email) == MTM_EMAIL_ALREADY_EXISTS){
        return MTM_EMAIL_ALREADY_EXISTS;
    }
    if( checkIfEscaperExist(escapeTechnion, email) == MTM_SUCCESS){
        return MTM_EMAIL_ALREADY_EXISTS;
    }

    return MTM_SUCCESS;
}

/**
 * Checks if room exists
 * @param escapeTechnion : system
 * @param faculty : faculty to which the room belongs
 * @param roomId : unique id of room
 * @return : ENUM
 */
static MtmErrorCode checkIfCompanyRoomExist(EscapeTechnion escapeTechnion, TechnionFaculty faculty, int roomId)
{
    TechnionFaculty tempFaculty;
    CompanyResult companyResult;

    SET_FOREACH(Company, company, escapeTechnion->companies){
        companyResult = getCompanyFaculty(company, &tempFaculty);
        if(companyResult != COMPANY_SUCCESS){
            return MTM_NULL_PARAMETER;
        }
        if(tempFaculty == faculty) {
            if(checkIfRoomExists(company, roomId) == COMPANY_ROOM_ID_ALREADY_EXIST) {
                return MTM_ID_ALREADY_EXIST;
            }
        }
    }

    return MTM_ID_DOES_NOT_EXIST;
}

/**
 * Checks if orders are not made for same escaper, day, and hour
 * @param escapeTechnion : system
 * @param escaperEmail : email of escaper
 * @param requestedDay
 * @param requestedTime
 * @return : ENUM
 */
static MtmErrorCode checkIfOrderNotCollide(EscapeTechnion escapeTechnion, char *escaperEmail,
                                           int requestedDay, int requestedTime)
{
    char *tempEmail = NULL;
    LIST_FOREACH(Order, order, escapeTechnion->orders){
        tempEmail = getOrderEmail(order);
        if(tempEmail == NULL){
            return MTM_OUT_OF_MEMORY;
        }
        if(strcmp(tempEmail, escaperEmail) == 0) {
            if (getOrderDay(order) == requestedDay && getOrderHour(order) == requestedTime) {
                free(tempEmail);
                return MTM_CLIENT_IN_ROOM;
            }
        }
    }

    free(tempEmail);
    return MTM_SUCCESS;
}

/**
 * Checks that company parameters are valid
 * @param companyEmail : email of company
 * @param companyFaculty : faculty of company
 * @return : ENUM
 */
static MtmErrorCode checkCompanyParameters(char *companyEmail, TechnionFaculty companyFaculty)
{
    if(companyEmail == NULL){
        return MTM_INVALID_PARAMETER;
    }
    if( checkEmail(companyEmail) != MTM_SUCCESS){
        return MTM_INVALID_PARAMETER;
    }
    if(companyFaculty == UNKNOWN){
        return MTM_INVALID_PARAMETER;
    }

    return MTM_SUCCESS;
}

/**
 * Checks if room is available
 * @param escapeTechnion : system
 * @param faculty : faculty to which the room belongs
 * @param roomId : unique id of room
 * @param req_hour : requested hour
 * @param req_day : requested day
 * @return : ENUM
 */
static MtmErrorCode checkIfRoomAvailable(EscapeTechnion escapeTechnion, TechnionFaculty faculty,
                                         int roomId, int req_hour, int req_day)
{
    int tempDay, tempHour, tempId;
    TechnionFaculty tempFaculty;
    CompanyResult companyResult;

    SET_FOREACH(Company, company, escapeTechnion->companies){
        getCompanyFaculty(company, &tempFaculty);
        assert(0 <= tempFaculty && tempFaculty < 18);
        if(tempFaculty == faculty){
            companyResult = checkIfCompanyRoomIsOpenById(company, roomId, req_hour);
            if(companyResult == COMPANY_ROOM_ID_DOES_NOT_EXIST){
                continue;
            }
            else if(companyResult == COMPANY_NO_ROOMS_AVAILABLE){
                return MTM_ROOM_NOT_AVAILABLE;
            }
            else if(companyResult == COMPANY_SUCCESS){
                break;
            }
        }
    }

    LIST_FOREACH(Order, order, escapeTechnion->orders){
        tempDay = getOrderDay(order);
        tempHour = getOrderHour(order);
        tempFaculty = getOrderRoomFaculty(order);
        tempId = getOrderRoomId(order);
        if(tempDay == req_day && tempHour == req_hour && tempFaculty  == faculty && tempId == roomId){
            return MTM_ROOM_NOT_AVAILABLE;
        }
    }

    return MTM_SUCCESS;
}

/**
 * Adds a new company to system
 * @param companyEmail
 * @param companyFaculty
 * @param escapeTechnion : system
 * @return : ENUM
 */
MtmErrorCode mtmCompanyAdd(char *companyEmail, TechnionFaculty companyFaculty, EscapeTechnion escapeTechnion)
{
    Company newCompany;
    CompanyResult companyResult;
    MtmErrorCode mtmErrorCode;
    SetResult setResult;

    mtmErrorCode = checkCompanyParameters(companyEmail, companyFaculty);
    if(mtmErrorCode != MTM_SUCCESS){
        return mtmErrorCode;
    }
    mtmErrorCode = mtmCheckIfEmailExists(escapeTechnion, companyEmail);
    if(mtmErrorCode != MTM_SUCCESS){
        return MTM_EMAIL_ALREADY_EXISTS;
    }

    newCompany = createCompany(companyEmail, companyFaculty, &companyResult);
    if(companyResult != COMPANY_SUCCESS){
        return translateCompanyResultToMtm(companyResult);
    }
    assert(newCompany != NULL);
    setResult = setAdd(escapeTechnion->companies, newCompany);
    if(setResult != SET_SUCCESS){
        freeCompany(newCompany);
        return translateSetResultToMtm(setResult);
    }

    return MTM_SUCCESS;
}

/**
 * Removes a company from the system
 * @param email : email of company to be removed
 * @param escapeTechnion : system
 * @return : ENUM
 */
MtmErrorCode mtmCompanyRemove(char *email, EscapeTechnion escapeTechnion)
{
    CompanyResult companyResult;
    char *emailIterator=NULL;

    if(escapeTechnion == NULL || email == NULL){
        return MTM_NULL_PARAMETER;
    }
    if( checkEmail(email) != MTM_SUCCESS ){
        return MTM_INVALID_PARAMETER;
    }
    
    SET_FOREACH(Company, company, escapeTechnion->companies){
        emailIterator = getCompanyEmail(company, &companyResult);
        if(companyResult != COMPANY_SUCCESS){
            return translateCompanyResultToMtm(companyResult);
        }
        if( strcmp(emailIterator, email) == 0 ){
            if(checkIfOrderExistForThisCompany(escapeTechnion, company) != MTM_SUCCESS){
                return MTM_RESERVATION_EXISTS;
            }
            setRemove(escapeTechnion->companies, company);
            free(emailIterator);
            return MTM_SUCCESS;
        }
    }

    free(emailIterator);

    return MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
}

/**
 * Adds a room to the system
 * @param email : email of company to which the room belongs
 * @param id : unique id of room
 * @param price : price of room
 * @param numOfPpl : number of people recommended
 * @param openHour : openning hour
 * @param closeHour : closing hour
 * @param difficulty
 * @param escapeTechnion : system
 * @return : ENUM
 */
MtmErrorCode mtmRoomAdd(char *email, int id, int price,int numOfPpl,int openHour,
                        int closeHour, int difficulty, EscapeTechnion escapeTechnion)
{
    Company company=NULL;
    CompanyResult companyResult;
    MtmErrorCode mtmErrorCode;
    TechnionFaculty companyFaculty;

    if(email == NULL){
        return MTM_INVALID_PARAMETER;
    }
    companyResult = mtmCheckCompanyRoomParameters(email, id, price, numOfPpl, openHour, closeHour, difficulty);
    if(companyResult == COMPANY_INVALID_PARAMETER){
        return MTM_INVALID_PARAMETER;
    }
    companyFaculty = mtmGetCompanyFacultyByEmail(escapeTechnion->companies, email, &mtmErrorCode);
    if(mtmErrorCode != MTM_SUCCESS){
        return mtmErrorCode;
    }
    mtmErrorCode = checkIfCompanyRoomExist(escapeTechnion, companyFaculty, id);
    if(mtmErrorCode == MTM_ID_ALREADY_EXIST){
        return mtmErrorCode;
    }
    company = mtmGetCompanyByEmail(email, escapeTechnion, &mtmErrorCode);
    if(mtmErrorCode != MTM_SUCCESS){
        return MTM_OUT_OF_MEMORY;
    }

    companyResult = addCompanyRoom( email, id, price, numOfPpl, openHour, closeHour, difficulty, company );
    if(companyResult != COMPANY_SUCCESS){
        return translateCompanyResultToMtm(companyResult);
    }

    return MTM_SUCCESS;
}

/**
 * Remove room from system
 * @param desiredFaculty : faculty to which the room belongs
 * @param id : unique id of room
 * @param escapeTechnion : system
 * @return : ENUM
 */
MtmErrorCode mtmRoomRemove(TechnionFaculty desiredFaculty, int id, EscapeTechnion escapeTechnion)
{
    TechnionFaculty companyFaculty;
    CompanyResult companyResult;
    MtmErrorCode mtmErrorCode;

    if(id <= 0){
        return MTM_INVALID_PARAMETER;
    }

    mtmErrorCode = checkIfOrderExistForThisRoom(escapeTechnion->orders, desiredFaculty, id);
    if(mtmErrorCode == MTM_RESERVATION_EXISTS){
        return mtmErrorCode;
    }

    SET_FOREACH(Company, company, escapeTechnion->companies){
        companyResult = getCompanyFaculty(company, &companyFaculty);
        if(companyResult != COMPANY_SUCCESS){
            return translateCompanyResultToMtm(companyResult);
        }
        if(companyFaculty == desiredFaculty){
            companyResult = companyRoomRemove(company, id);
            if(companyResult == COMPANY_SUCCESS){
                return MTM_SUCCESS;
            }
        }
    }

    return MTM_ID_DOES_NOT_EXIST;
}

/**
 * Add escaper to system
 * @param email : email of client
 * @param faculty : faculty to which the escaper belongs
 * @param skill : skill of escaper
 * @param escapeTechnion : system
 * @return : ENUM
 */
MtmErrorCode mtmEscaperAdd(char *email, TechnionFaculty faculty, int skill, EscapeTechnion escapeTechnion)
{
    EscaperResult escaperResult;
    SetResult setResult;
    Escaper newEscaper = NULL;
    MtmErrorCode mtmErrorCode;

    newEscaper = createEscaper(email, faculty, skill, &escaperResult);
    if(escaperResult != ESCAPER_SUCCESS){
        return translateEscaperResultToMtm(escaperResult);
    }
    mtmErrorCode = mtmCheckIfEmailExists(escapeTechnion, email);
    if(mtmErrorCode != MTM_SUCCESS){
        return MTM_EMAIL_ALREADY_EXISTS;
    }

    setResult = setAdd( escapeTechnion->escapers, newEscaper);
    freeEscaper(newEscaper);
    if(setResult != SET_SUCCESS){
        return translateSetResultToMtm(setResult);
    }

    return MTM_SUCCESS;
}

/**
 * Removes escpaer form system
 * @param email : email of escpaer
 * @param escapeTechnion : system
 * @return : ENUM
 */
MtmErrorCode mtmEscaperRemove(char *email, EscapeTechnion escapeTechnion)
{
    char *escaperEmail = NULL;
    EscaperResult escaperResult;

    if(checkEmail(email) != MTM_SUCCESS){
        return MTM_INVALID_PARAMETER;
    }

    SET_FOREACH(Escaper, escaper, escapeTechnion->escapers){
        escaperEmail = escaperGetEmail(escaper, &escaperResult);
        if(escaperEmail == NULL){
            return MTM_OUT_OF_MEMORY;
        }
        assert(escaperEmail != NULL);
        if( strcmp(escaperEmail, email) == 0){
            setRemove(escapeTechnion->escapers, escaper);
            free(escaperEmail);

            return MTM_SUCCESS;
        }
    }

    return MTM_CLIENT_EMAIL_DOES_NOT_EXIST;
}

/**
 * Creates a new order
 * @param escaperEmail
 * @param companyFaculty : faculty which owns the room
 * @param roomId : unique id of room
 * @param requestedTime
 * @param requestedDay
 * @param numOfPpl : effective number of peolpe to enter room
 * @param escapeTechnion : system
 * @return : ENUM
 */
MtmErrorCode mtmEscaperOrder(char *escaperEmail, TechnionFaculty companyFaculty, int roomId, int requestedTime,
                             int requestedDay, int numOfPpl, EscapeTechnion escapeTechnion)
{
    Order newOrder = NULL;
    char *companyEmail = NULL;
    int roomPrice;
    orderResult orderResult;
    ListResult listResult;
    MtmErrorCode mtmErrorCode;
    TechnionFaculty escaperFaculty;

    if(numOfPpl <= 0 || requestedTime < 0 || requestedTime > 23 || requestedDay < 0 || companyFaculty == UNKNOWN){
        return MTM_INVALID_PARAMETER;
    }
    if(checkEmail(escaperEmail) != MTM_SUCCESS){
        return MTM_INVALID_PARAMETER;
    }
    if(checkIfEscaperExist(escapeTechnion, escaperEmail) == MTM_CLIENT_EMAIL_DOES_NOT_EXIST){
        return MTM_CLIENT_EMAIL_DOES_NOT_EXIST;
    }
    if(checkIfCompanyRoomExist(escapeTechnion, companyFaculty, roomId) == MTM_ID_DOES_NOT_EXIST){
        return MTM_ID_DOES_NOT_EXIST;
    }
    if(checkIfOrderNotCollide(escapeTechnion, escaperEmail, requestedDay, requestedTime) != MTM_SUCCESS){
        return MTM_CLIENT_IN_ROOM;
    }
    if(checkIfRoomAvailable(escapeTechnion, companyFaculty, roomId, requestedTime, requestedDay) != MTM_SUCCESS){
        return MTM_ROOM_NOT_AVAILABLE;
    }
    companyEmail = mtmGetCompanyEmailByFaculty(escapeTechnion->companies, companyFaculty);
    if(companyEmail == NULL){
        return MTM_OUT_OF_MEMORY;
    }
    roomPrice = mtmGetRoomPriceByFacultyAndId(escapeTechnion->companies, companyFaculty, roomId);
    //roomNumOfPPl = mtmGetRoomNumOfPplByFacultyAndId(escapeTechnion->companies, companyFaculty, roomId);
    mtmErrorCode = mtmGetEscaperFacultyByEmail(escapeTechnion->escapers, escaperEmail, &escaperFaculty);
    if(mtmErrorCode != MTM_SUCCESS){
        free(companyEmail);
        return mtmErrorCode;
    }

    newOrder = createOrder(escaperEmail, escaperFaculty, companyFaculty, roomId, numOfPpl,
                           requestedTime, requestedDay, roomPrice, &orderResult);

    if(orderResult != ORDER_SUCCESS){
        free(companyEmail);
        return translateOrderResultToMtm(orderResult);
    }
    assert(newOrder != NULL);
    listResult = listInsertLast(escapeTechnion->orders, newOrder);
    if(listResult != LIST_SUCCESS){
        free(companyEmail);
        return translateListResultToMtm(listResult);
    }

    free(companyEmail);
    return MTM_SUCCESS;
}

/**
 * Recommends a room for an escaper
 * @param escaperEmail
 * @param numOfPpl : number of people to enter room
 * @param escapeTechnion : system
 * @return : ENUM
 */
MtmErrorCode mtmEscaperRecommend(char *escaperEmail, int numOfPpl, EscapeTechnion escapeTechnion)
{

    int recommendedRoomId, tempRecommendedRoomId, skill_level, roomPrice, set_size=0, calcResult ;
    double calculation=DBL_MAX, tempCalculation = DBL_MAX;
    TechnionFaculty companyFaculty, tempCompanyFaculty, escaperFaculty;
    orderResult orderResult;
    CompanyResult companyResult;
    MtmErrorCode mtmErrorCode;
    ListResult listResult;
    Order new_order;


    if(escaperEmail == NULL || escapeTechnion == NULL){
        return MTM_INVALID_PARAMETER;
    }

    mtmErrorCode = checkEmail(escaperEmail);
    if(mtmErrorCode != MTM_SUCCESS){
        return MTM_INVALID_PARAMETER;
    }

    mtmErrorCode = checkIfEscaperExist(escapeTechnion, escaperEmail);
    if(mtmErrorCode == MTM_CLIENT_EMAIL_DOES_NOT_EXIST){
        return MTM_CLIENT_EMAIL_DOES_NOT_EXIST;
    }

    SET_FOREACH(Company, company, escapeTechnion->companies){
        set_size = getCompanyRoomListSize(company);
        if(set_size > 0){
            break;
        }
    }
    if(set_size == 0){
        return MTM_NO_ROOMS_AVAILABLE;
    }

    skill_level = mtmGetEscaperSkillByEmail(escapeTechnion, escaperEmail);
    mtmErrorCode = mtmGetEscaperFacultyByEmail(escapeTechnion->escapers, escaperEmail, &escaperFaculty);
    if(mtmErrorCode != MTM_SUCCESS){
        return mtmErrorCode;
    }

    SET_FOREACH(Company, company, escapeTechnion->companies){
        tempRecommendedRoomId = getCompanyRecommendedRoomId(company, numOfPpl, skill_level, &tempCalculation);
        getCompanyFaculty(company, &tempCompanyFaculty);
        if(tempCalculation < calculation){
            calculation = tempCalculation;
            recommendedRoomId = tempRecommendedRoomId;
            companyFaculty = tempCompanyFaculty;
            roomPrice = getCompanyRoomPriceById(company, recommendedRoomId);
        }
        else if(tempCalculation == calculation){
            mtmGetEscaperFacultyByEmail(escapeTechnion->escapers, escaperEmail, &escaperFaculty);
            calcResult = calculateFacultyDistanceFromClient(escaperFaculty, companyFaculty, tempCompanyFaculty,
                                                                tempRecommendedRoomId,recommendedRoomId,&companyResult);
            if(companyResult == COMPANY_CALC_BY_FACULTY){
                companyFaculty = (TechnionFaculty)calcResult;
                if(companyFaculty == tempCompanyFaculty){
                    recommendedRoomId = tempRecommendedRoomId;
                    roomPrice = getCompanyRoomPriceById(company, recommendedRoomId);
                }
            }
            else {
                recommendedRoomId = calcResult;
                if(recommendedRoomId == tempRecommendedRoomId){
                    companyFaculty = tempCompanyFaculty;
                    roomPrice = getCompanyRoomPriceById(company, recommendedRoomId);
                }
            }

        }
    }

    for(int day=0; day < MAX_DAYS; day++){
        for(int time=0; time < MAX_HOUR; time++){
            assert(recommendedRoomId > 0 || roomPrice > 0);
            new_order = createOrder(escaperEmail, escaperFaculty, companyFaculty, recommendedRoomId, numOfPpl,
                                    time, day, roomPrice, &orderResult);
            if(orderResult == ORDER_SUCCESS){
               listResult = listInsertFirst(escapeTechnion->orders, new_order);
                if(listResult != LIST_SUCCESS){
                    freeOrder(new_order);
                    return translateListResultToMtm(listResult);
                }

            }
            return MTM_SUCCESS;
        }
    }

   return MTM_SUCCESS;
}

/**
 * Prints daily report and moves system a day forward
 * @param escapeTechnion : system
 * @return : ENUM
 */
MtmErrorCode mtmReportDay(EscapeTechnion escapeTechnion)
{
    List orders_arrived, orders_not_arrived;
    int escaperSkill, roomDifficulty, roomId, price;
    char *companyEmail = NULL, *escaperEmail = NULL;
    TechnionFaculty roomFaculty;

    LIST_FOREACH(Order, order, escapeTechnion->orders){
        increaseOrderDay(order);
    }

    orders_arrived = createOrderDayArrivedFilteredList(escapeTechnion->orders);
    if(orders_arrived == NULL){
        return MTM_OUT_OF_MEMORY;
    }

    //need to add a secondary sort also third
    sortOrdersByHour(orders_arrived);

    mtmPrintDayHeader(escapeTechnion->outputStream, escapeTechnion->day, listGetSize(orders_arrived));
    LIST_FOREACH(Order, order, orders_arrived){
        escaperEmail = getOrderEmail(order);
        if(escaperEmail == NULL){
            return MTM_OUT_OF_MEMORY;
        }
        escaperSkill = mtmGetEscaperSkillByEmail(escapeTechnion, escaperEmail);
        roomId = getOrderRoomId(order);
        roomFaculty = getOrderRoomFaculty(order);
        roomDifficulty = mtmGetRoomDifficultyByFacultyAndId(escapeTechnion->companies, roomFaculty, roomId);
        price = getOrderPrice(order);
        companyEmail = mtmGetCompanyEmailByFaculty(escapeTechnion->companies, roomFaculty);
        assert(roomId != -1 || roomFaculty != -1 || roomDifficulty != -1 || price >= 0);
        escapeTechnion->total_revenue[roomFaculty] += price;
        printOrder(escapeTechnion->outputStream, order, escaperSkill, roomDifficulty, companyEmail);
    }
    mtmPrintDayFooter(escapeTechnion->outputStream, escapeTechnion->day);
    free(escaperEmail);
    free(companyEmail);

    //remove all orders that arrived
    orders_not_arrived = createOrderDayNotArrivedFilteredList(escapeTechnion->orders);
    if(orders_not_arrived == NULL){
        return MTM_OUT_OF_MEMORY;
    }
    listDestroy(escapeTechnion->orders);
    escapeTechnion->orders = orders_not_arrived;
    escapeTechnion->day++;

    return MTM_SUCCESS;
}

/**
 * Prints best faculties
 * @param escapeTechnion : system
 * @return : ENUM
 */
MtmErrorCode mtmReportBest(EscapeTechnion escapeTechnion)
{
    int best_faculty = 0, second_faculty = 0, third_faculty = 0, total_revenue = 0;
    int *revenues = escapeTechnion->total_revenue;

    total_revenue = mtmGetTotalRevenue(escapeTechnion);
    //find best faculty
    for(int i=0; i < UNKNOWN; i++){
        if(revenues[best_faculty] < revenues[i]){
            best_faculty = i;
        }
    }
    //find second_faculty
    if(best_faculty == 0){
        second_faculty++;
    }
    for (int i = 0; i < UNKNOWN; i++) {
        if(i == best_faculty){
            continue;
        }
        if(revenues[second_faculty] < revenues[i]){
            second_faculty = i;
        }
    }
    if(second_faculty == 1){
        third_faculty = 2;
    }
    //find third_faculty
    for (int i = 0; i < UNKNOWN; i++) {
        if(i == second_faculty){
            continue;
        }
        if(revenues[third_faculty] < revenues[i]){
            third_faculty = i;
        }
    }
    mtmPrintFacultiesHeader(escapeTechnion->outputStream, UNKNOWN, escapeTechnion->day, total_revenue);
    mtmPrintFaculty(escapeTechnion->outputStream,  (TechnionFaculty)best_faculty,  revenues[best_faculty]);
    mtmPrintFaculty(escapeTechnion->outputStream,  (TechnionFaculty)second_faculty,  revenues[second_faculty]);
    mtmPrintFaculty(escapeTechnion->outputStream,  (TechnionFaculty)third_faculty,  revenues[third_faculty]);
    mtmPrintFacultiesFooter(escapeTechnion->outputStream);

    return MTM_SUCCESS;
}

