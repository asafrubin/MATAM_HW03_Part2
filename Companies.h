#ifndef HW03_PART2_COMPANIES_H
#define HW03_PART2_COMPANIES_H

#include "mtm_ex3.h"

typedef struct SCompany *Company;

typedef enum {
    CMP_OUT_OF_MEMORY, // You should exit program after this error
    CMP_NULL_PARAMETER,
    CMP_INVALID_PARAMETER,
    CMP_SUCCESS,
} CompanyResult;

int compareCompanies(Company firstCompany , Company secondCompany);

void freeCompany(Company company);

Company copyCompany(Company company);

CompanyResult createCompany(char *email, TechnionFaculty faculty, Company newCompany);

CompanyResult getCompanyRooms(Company targetCompany , void *rooms);


#endif //HW03_PART2_COMPANIES_H
