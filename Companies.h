#ifndef HW03_PART2_COMPANIES_H
#define HW03_PART2_COMPANIES_H

#include "mtm_ex3.h"

typedef struct SCompany *Company;

typedef enum {
    COMPANY_OUT_OF_MEMORY, // You should exit program after this error
    COMPANY_NULL_PARAMETER,
    COMPANY_INVALID_PARAMETER,
    COMPANY_ROOM_ID_ALREADY_EXIST,
    COMPANY_ROOM_ID_DOES_NOT_EXIST,
    COMPANY_NO_ROOMS_AVAILABLE,
    COMPANY_EMAIL_DOES_NOT_EXIST,
    COMPANY_SUCCESS
} CompanyResult;

int getCompanyRecommendedRoomId(Company company, int P_e, int skill_level, double *best_calculation);

CompanyResult checkIfRoomExists(Company company , int id );

int compareCompanies(Company firstCompany , Company secondCompany);

int getCompanyRoomListSize(Company company);

void freeCompany(Company company);

char *getCompanyEmail(Company company, CompanyResult *result);

CompanyResult getCompanyFaculty(Company company, TechnionFaculty *faculty);

Company copyCompany(Company company);

Company createCompany(char *email, TechnionFaculty faculty, CompanyResult *result);

CompanyResult getCompanyRooms(Company targetCompany , void *rooms);

CompanyResult addCompanyRoom(char *companyEmail,int roomId,int roomPrice,int num_ppl,int open_time,
                             int close_time,int diffuculty, Company company);

CompanyResult companyRoomRemove(Company company, int roomId);

//void setRemoveRoom(SetElement room);

//SetElement setCopyOfRoom(SetElement roomToCopy);

//int setCompareRoom(SetElement firstRoom, SetElement secondRoom);


#endif //HW03_PART2_COMPANIES_H
