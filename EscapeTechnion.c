#include "EscapeTechnion.h"
#include "set.h"
#include "list.h"
#include "Companies.h"
#include "Escapers.h"
#include "Orders.h"
#include "Rooms.h"
#include "test_utilities.h"
#include <limits.h>
#include <mem.h>
#include <stdlib.h>
#include <float.h>

#define MAX_DAYS 365
#define MAX_HOUR 23

struct SEscapeTechnion{
    FILE *outputStream;
    int day;
    Set escapers;
    Set companies;
    List orders;
};

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

static int getEscaperSkill(EscapeTechnion escapeTechnion, char *escaperEmail)
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

static Company getCompanyByEmail(char *email, EscapeTechnion escapeTechnion, MtmErrorCode *mtmResult)
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

    *mtmResult = MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
    return NULL;
}

static MtmErrorCode checkEmail(char *name)
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
        return MTM_INVALID_PARAMETER;
    }

    return MTM_SUCCESS;
}

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

static MtmErrorCode checkIfRoomAvailable(EscapeTechnion escapeTechnion, TechnionFaculty faculty,
                                         int roomId, int req_hour, int req_day)
{

    LIST_FOREACH(Order, order, escapeTechnion->orders){
        if(getOrderDay(order) == req_day && getOrderHour(order) == req_hour && getOrderFaculty(order) == faculty
                && getOrderRoomId(order) == roomId){
            return MTM_ROOM_NOT_AVAILABLE;
        }
    }

    return MTM_SUCCESS;
}

MtmErrorCode mtmCompanyAdd(char *companyEmail, TechnionFaculty companyFaculty, EscapeTechnion escapeTechnion)
{
    Company newCompany;
    CompanyResult companyResult;
    SetResult setResult;

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
            setRemove(escapeTechnion->companies, company);
            free(emailIterator);
            return MTM_SUCCESS;
        }
    }

    free(emailIterator);

    return MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
}

MtmErrorCode mtmRoomAdd(char *email, int id, int price,int numOfPpl,int openHour,
                        int closeHour, int difficulty, EscapeTechnion escapeTechnion)
{
    Company company=NULL;
    CompanyResult companyResult;
    MtmErrorCode mtmErrorCode;

    //check if email exist
    company = getCompanyByEmail(email, escapeTechnion, &mtmErrorCode);
    if(mtmErrorCode != MTM_SUCCESS){
        return mtmErrorCode;
    }
    assert(company != NULL);

    companyResult = addCompanyRoom(email, id, price, numOfPpl, openHour, closeHour, difficulty, company );
    if(companyResult != COMPANY_SUCCESS){
        return translateCompanyResultToMtm(companyResult);
    }

    return MTM_SUCCESS;
}

MtmErrorCode mtmRoomRemove(TechnionFaculty desiredFaculty, int id, EscapeTechnion escapeTechnion)
{
    TechnionFaculty companyFaculty;
    CompanyResult companyResult;

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

MtmErrorCode mtmEscaperAdd(char *email, TechnionFaculty faculty, int skill, EscapeTechnion escapeTechnion)
{
    EscaperResult escaperResult;
    SetResult setResult;
    Escaper newEscaper = NULL;

    newEscaper = createEscaper(email, faculty, skill, &escaperResult);
    if(escaperResult != ESCAPER_SUCCESS){
        return (MtmErrorCode)escaperResult; // need to write a function to convert
    }

    setResult = setAdd( escapeTechnion->escapers, newEscaper);
    freeEscaper(&newEscaper);

    if(setResult != SET_SUCCESS){
        return (MtmErrorCode)setResult;
    }

    return MTM_SUCCESS;
}

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

MtmErrorCode mtmEscaperOrder(char *escaperEmail, TechnionFaculty faculty, int roomId, int requestedTime,
                             int requestedDay, int numOfPpl, EscapeTechnion escapeTechnion)
{
    Order newOrder = NULL;
    orderResult orderResult;
    ListResult listResult;

    if(numOfPpl < 0){
        return MTM_INVALID_PARAMETER;
    }
    if(requestedTime < 0 || requestedTime > 23){
        return MTM_INVALID_PARAMETER;
    }
    if( requestedDay < 0){
        return MTM_INVALID_PARAMETER;
    }
    if(checkEmail(escaperEmail) != MTM_SUCCESS){
        return MTM_INVALID_PARAMETER;
    }
    if(checkIfEscaperExist(escapeTechnion, escaperEmail) == MTM_CLIENT_EMAIL_DOES_NOT_EXIST){
        return MTM_CLIENT_EMAIL_DOES_NOT_EXIST;
    }
    if(checkIfCompanyRoomExist(escapeTechnion, faculty, roomId) == MTM_ID_DOES_NOT_EXIST){
        return MTM_ID_DOES_NOT_EXIST;
    }
    if(checkIfOrderNotCollide(escapeTechnion, escaperEmail, requestedDay, requestedTime) != MTM_SUCCESS){
        return MTM_CLIENT_IN_ROOM;
    }
    if(checkIfRoomAvailable(escapeTechnion, faculty, roomId, requestedDay, requestedTime) != MTM_SUCCESS){
        return MTM_ROOM_NOT_AVAILABLE;
    }

    newOrder = createOrder(escaperEmail, faculty, roomId, numOfPpl, requestedTime, requestedDay,
                           escapeTechnion->day, &orderResult);
    if(orderResult != ORDER_SUCCESS){
        return translateOrderResultToMtm(orderResult);
    }
    assert(newOrder != NULL);
    listResult = listInsertLast(escapeTechnion->orders, newOrder);
    if(listResult != LIST_SUCCESS){
        return translateListResultToMtm(listResult);
    }

    return MTM_SUCCESS;
}

MtmErrorCode mtmEscaperRecommend(char *escaperEmail, int numOfPpl, EscapeTechnion escapeTechnion)
{

    int recommendedRoomId, tempRecommendedRoomId, skill_level, set_size=0;
    double calculation=DBL_MAX, tempCalculation;
    TechnionFaculty faculty;
    orderResult orderResult;
    ListResult listResult;
    Order new_order;


    if(escaperEmail == NULL || escapeTechnion == NULL){
        return MTM_NULL_PARAMETER;
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

    skill_level = getEscaperSkill(escapeTechnion, escaperEmail);

    SET_FOREACH(Company, company, escapeTechnion->companies){
        tempRecommendedRoomId = getCompanyRecommendedRoomId(company, numOfPpl, skill_level, &tempCalculation);
        if(tempCalculation < calculation){
            calculation = tempCalculation;
            recommendedRoomId = tempRecommendedRoomId;
            getCompanyFaculty(company, &faculty);
        }
    }

    for(int day=0; day < MAX_DAYS; day++){
        for(int time=0; time < MAX_HOUR; time++){
            new_order = createOrder(escaperEmail, faculty, recommendedRoomId, numOfPpl, time, day, &orderResult);
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

MtmErrorCode mtmReportDay(EscapeTechnion escapeTechnion)
{
    escapeTechnion->day++;

    LIST_FOREACH(Order, order, escapeTechnion->orders){
        increaseOrderDay(order);
    }



}