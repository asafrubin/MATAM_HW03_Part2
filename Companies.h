#ifndef HW03_PART2_COMPANIES_H
#define HW03_PART2_COMPANIES_H

#include "mtm_ex3.h"
#include "set.h"

typedef struct SCompany *Company;

typedef enum {
    COMPANY_OUT_OF_MEMORY, // You should exit program after this error
    COMPANY_NULL_PARAMETER,
    COMPANY_INVALID_PARAMETER,
    COMPANY_ROOM_ID_ALREADY_EXIST,
    COMPANY_ROOM_ID_DOES_NOT_EXIST,
    COMPANY_NO_ROOMS_AVAILABLE,
    COMPANY_EMAIL_DOES_NOT_EXIST,
    COMPANY_CALC_BY_FACULTY,
    COMPANY_CALC_BY_ID,
    COMPANY_SUCCESS
} CompanyResult;

int getCompanyRecommendedRoomId(Company company, int P_e, int skill_level, int *best_calculation,
                                TechnionFaculty escaperFaculty, TechnionFaculty *companyFaculty,
                                TechnionFaculty *tempCompanyFaculty, int *roomId, int *roomPrice);

int getCompanyRoomPriceById(Company company, int roomId);

int getCompanyRoomNumOfPplById(Company company, int roomId);

int getCompanyRoomDifficultyById(Company company, int roomId);

CompanyResult checkIfRoomExists(Company company , int id );

int compareCompanies(Company firstCompany , Company secondCompany);

CompanyResult mtmCheckCompanyRoomParameters(char *companyEmail, int roomId, int roomPrice, int num_ppl, int open_time,
                                            int close_time, int diffuculty);

int setCompareCompanies(SetElement firstCompany, SetElement secondCompany);

int getCompanyRoomListSize(Company company);

void freeCompany(Company company);

void setFreeCompany(SetElement company);

char *getCompanyEmail(Company company, CompanyResult *result);

CompanyResult getCompanyFaculty(Company company, TechnionFaculty *faculty);

Company copyCompany(Company company);

SetElement setCopyCompany(SetElement company);

Company createCompany(char *email, TechnionFaculty faculty, CompanyResult *result);

//CompanyResult getCompanyRooms(Company targetCompany , void *rooms);

CompanyResult addCompanyRoom(char *companyEmail,int roomId,int roomPrice,int num_ppl,int open_time,
                             int close_time,int diffuculty, Company company);

CompanyResult companyRoomRemove(Company company, int roomId);

CompanyResult checkIfCompanyRoomIsOpenById(Company company, int roomId, int req_hour);

//void setRemoveRoom(SetElement room);

//SetElement setCopyOfRoom(SetElement roomToCopy);

//int setCompareRoom(SetElement firstRoom, SetElement secondRoom);


#endif //HW03_PART2_COMPANIES_H
