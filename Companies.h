#ifndef HW03_PART2_COMPANIES_H
#define HW03_PART2_COMPANIES_H

#include "mtm_ex3.h"

typedef struct SCompany *Company;

typedef enum {
    COMPANY_OUT_OF_MEMORY, // You should exit program after this error
    COMPANY_NULL_PARAMETER,
    COMPANY_INVALID_PARAMETER,
    ROOMS_ID_ALREADY_EXIST,
    ROOMS_ID_DOES_NOT_EXIST,
    ROOMS_NO_ROOMS_AVAILABLE,
    COMPANY_EMAIL_DOES_NOT_EXIST,
    COMPANY_SUCCESS
} CompanyResult;


CompanyResult createCompany(char *email, TechnionFaculty faculty, Company newCompany);

CompanyResult checkIfRoomExists(Company company , int id);

CompanyResult getRoomPriceFromCompany(Company company , int id , int *price);

CompanyResult addRoomToCompany(Company company , int id , int price , int num_ppl , int start_time , int
finish_time , int difficulty);

int compareCompanies(Company firstCompany , Company secondCompany);

void freeCompany(Company company);

Company copyCompany(Company company);

TechnionFaculty getFaculty(Company company);


#endif //HW03_PART2_COMPANIES_H
