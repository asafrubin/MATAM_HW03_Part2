#include "EscapeTechnion.h"
#include "set.h"
#include "list.h"
#include "Companies.h"
#include "Escapers.h"
#include "Orders.h"
#include "test_utilities.h"
#include <limits.h>
#include <string.h>
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
        escapeTechnion->total_revenue[i] = 0;
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

void mtmDestroyEscapeTechnion(EscapeTechnion escapeTechnion)
{
    if(escapeTechnion) {
        setDestroy(escapeTechnion->companies);
        setDestroy(escapeTechnion->escapers);
        listDestroy(escapeTechnion->orders);
        free(escapeTechnion->total_revenue);
        free(escapeTechnion);
    }
}

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

/*
static int getCompanyRoomPriceByIdAndFaculty(EscapeTechnion  escapeTechnion, TechnionFaculty companyFaculty,
                                                      int roomId)
{
    TechnionFaculty tempFaculty;
    int price;

    SET_FOREACH(Company, company, escapeTechnion->companies){
        getCompanyFaculty(company, &tempFaculty);
        if(tempFaculty == companyFaculty){
           price = getCompanyRoomPriceById(company, roomId);
            if(price != -1 ){
                return price;
            }
        }
    }
    return -1;
}
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

static TechnionFaculty mtmGetCompanyFacultyByEmail(Set companies, char *email, MtmErrorCode *mtmErrorCode)
{
    char *tempEmail=NULL;
    CompanyResult companyResult;
    TechnionFaculty faculty;

    SET_FOREACH(Company, company, companies){
        tempEmail = getCompanyEmail(company, &companyResult);
        assert(tempEmail != NULL);
        if(strcmp(tempEmail, email) == 0 ){
            companyResult = getCompanyFaculty(company, &faculty);
            assert(companyResult == COMPANY_SUCCESS);
            *mtmErrorCode = MTM_SUCCESS;
            free(tempEmail);
            return faculty;
        }
    }

    *mtmErrorCode = MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
    free(tempEmail);
    return UNKNOWN;
}

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

static int mtmGetCompanyRecommendedRoomId(EscapeTechnion escapeTechnion,int numOfPpl,char *escaperEmail,
                                          TechnionFaculty *companyFaculty, TechnionFaculty escaperFaculty, int *roomId,
                                          int *roomPrice, CompanyResult *companyResult)
{
    int best_id=0, skill_level=0, calculation=INT_MAX;
    skill_level = mtmGetEscaperSkillByEmail(escapeTechnion, escaperEmail);
    TechnionFaculty tempCompanyFaculty;

    SET_FOREACH(Company, company, escapeTechnion->companies){
        getCompanyFaculty(company, &tempCompanyFaculty);
        best_id = getCompanyRecommendedRoomId(company, numOfPpl, skill_level, &calculation, escaperFaculty,
                                             companyFaculty, &tempCompanyFaculty, roomId, roomPrice);

    }

    return best_id;
}

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

    freeCompany(newCompany);
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

MtmErrorCode mtmEscaperRemove(char *email, EscapeTechnion escapeTechnion)
{
    char *escaperEmail = NULL;
    EscaperResult escaperResult;

    if(checkEmail(email) != MTM_SUCCESS){
        return MTM_INVALID_PARAMETER;
    }
    removeOrderOfEscaper(escapeTechnion->orders, email);

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

    freeOrder(newOrder);
    free(companyEmail);
    return MTM_SUCCESS;
}

MtmErrorCode mtmEscaperRecommend(char *escaperEmail, int numOfPpl, EscapeTechnion escapeTechnion)
{

    int recommendedRoomId=0, roomPrice=0, set_size=0;
    TechnionFaculty companyFaculty, escaperFaculty;
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

    mtmErrorCode = mtmGetEscaperFacultyByEmail(escapeTechnion->escapers, escaperEmail, &escaperFaculty);
    if(mtmErrorCode != MTM_SUCCESS){
        return mtmErrorCode;
    }

    mtmGetCompanyRecommendedRoomId(escapeTechnion, numOfPpl, escaperEmail, &companyFaculty, escaperFaculty,
                                   &recommendedRoomId, &roomPrice, &companyResult );


    /*
    SET_FOREACH(Company, company, escapeTechnion->companies){
        if(tempRecommendedRoomId != 0){
            tempRecommendedRoomId2 = tempRecommendedRoomId;
        }
        tempRecommendedRoomId = getCompanyRecommendedRoomId(company, numOfPpl, skill_level, &tempCalculation);
        getCompanyFaculty(company, &tempCompanyFaculty);
        if(tempCalculation < calculation){
            calculation = tempCalculation;
            recommendedRoomId = tempRecommendedRoomId;
            companyFaculty = tempCompanyFaculty;
            roomPrice = getCompanyRoomPriceById(company, recommendedRoomId);
        }
        else if(tempCalculation == calculation){
            calcResult = calculateFacultyDistanceFromClient(escaperFaculty, companyFaculty, tempCompanyFaculty,
                                                                tempRecommendedRoomId2,recommendedRoomId,&companyResult);
            if(companyResult == COMPANY_CALC_BY_FACULTY){
                companyFaculty = (TechnionFaculty)calcResult;
                if(companyFaculty == tempCompanyFaculty){
                    recommendedRoomId = tempRecommendedRoomId2;
                    roomPrice = getCompanyRoomPriceByIdAndFaculty(escapeTechnion, companyFaculty, recommendedRoomId);
                }
            }
            else {
                recommendedRoomId = calcResult;
                if(recommendedRoomId == tempRecommendedRoomId2){
                    companyFaculty = tempCompanyFaculty;
                    roomPrice = getCompanyRoomPriceByIdAndFaculty(escapeTechnion, companyFaculty, recommendedRoomId);
                }
            }

        }
    }
     */
    assert(price != -1);

    for(int day=0; day < MAX_DAYS; day++){
        for(int time=0; time < MAX_HOUR; time++){
            assert(recommendedRoomId > 0 || roomPrice > 0);
            if(checkIfRoomAvailable(escapeTechnion, companyFaculty, recommendedRoomId, time, day) != MTM_SUCCESS){
                continue;
            }
            new_order = createOrder(escaperEmail, escaperFaculty, companyFaculty, recommendedRoomId, numOfPpl,
                                    time, day, roomPrice, &orderResult);
            if(orderResult == ORDER_SUCCESS){
               listResult = listInsertFirst(escapeTechnion->orders, new_order);
                if(listResult != LIST_SUCCESS){
                    freeOrder(new_order);
                    return translateListResultToMtm(listResult);
                }

            }
            freeOrder(new_order);
            return MTM_SUCCESS;
        }
    }

   return MTM_SUCCESS;
}

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

    sortOrdersByHour(orders_arrived);

    mtmPrintDayHeader(escapeTechnion->outputStream, escapeTechnion->day, listGetSize(orders_arrived));
    LIST_FOREACH(Order, order, orders_arrived){
        escaperEmail = getOrderEmail(order);
        if(escaperEmail == NULL){
            listDestroy(orders_arrived);
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
    listDestroy(orders_arrived);

    //remove all orders that arrived
    escapeTechnion->day++;
    orders_not_arrived = createOrderDayNotArrivedFilteredList(escapeTechnion->orders);
    if(orders_not_arrived == NULL){
        return MTM_OUT_OF_MEMORY;
    }
    listDestroy(escapeTechnion->orders);
    escapeTechnion->orders = orders_not_arrived;


    return MTM_SUCCESS;
}

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
    if(best_faculty == 0){
        third_faculty++;
    }
    if(second_faculty == 1){
        third_faculty++;
    }
    //find third_faculty
    for (int i = third_faculty; i < UNKNOWN; i++) {
        if(revenues[third_faculty] < revenues[i]){
            if(i == second_faculty || i == best_faculty){
                continue;
            }
            third_faculty = i;
        }
        if(revenues[third_faculty] == revenues[i]){

        }

    }

    mtmPrintFacultiesHeader(escapeTechnion->outputStream, UNKNOWN, escapeTechnion->day, total_revenue);
    mtmPrintFaculty(escapeTechnion->outputStream,  (TechnionFaculty)best_faculty,  revenues[best_faculty]);
    mtmPrintFaculty(escapeTechnion->outputStream,  (TechnionFaculty)second_faculty,  revenues[second_faculty]);
    mtmPrintFaculty(escapeTechnion->outputStream,  (TechnionFaculty)third_faculty,  revenues[third_faculty]);
    mtmPrintFacultiesFooter(escapeTechnion->outputStream);

    return MTM_SUCCESS;
}

