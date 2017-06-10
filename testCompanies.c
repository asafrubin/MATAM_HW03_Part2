#include "test_utilities.h"
#include "mtm_ex3.h"
#include "Companies.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


static bool testCreateCompany() {
    Company newCompany = NULL;
    CompanyResult companyResult;
    TechnionFaculty faculty = COMPUTER_SCIENCE;

    char *goodEmail = "asaf@gmail.com";//, *badEmail1 = "asaf@@gmail.com", *badEmail2 = "asaf";

    newCompany = createCompany(goodEmail,faculty, &companyResult);
    ASSERT_TEST(companyResult == COMPANY_SUCCESS);
    freeCompany(newCompany);

    return true;
}

static bool testAddRoom()
{
    char *goodEmail = "asaf@gmail.com";
    char *badEmail1 = "nate@@gmail.com";
    char *badEmail2 = "BadEmail";

    int id = 5, price = 4*15, num_ppl = 4, open_time = 12, close_time = 16, goodDifficulty = 8, badDifficulty = 12;
    CompanyResult result;
    Company company;

    company = createCompany(goodEmail, COMPUTER_SCIENCE, &result);
    ASSERT_TEST(result == COMPANY_SUCCESS);

    result = addCompanyRoom(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty, company);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    companyRoomRemove(company, id);
    result = addCompanyRoom(badEmail1, id, price, num_ppl, open_time, close_time, goodDifficulty, company);
    ASSERT_TEST(result == COMPANY_INVALID_PARAMETER);
    companyRoomRemove(company, id);
    result = addCompanyRoom(goodEmail, id, price, num_ppl, open_time, close_time, badDifficulty, company);
    ASSERT_TEST(result == COMPANY_INVALID_PARAMETER);
    companyRoomRemove(company, id);
    result = addCompanyRoom(badEmail2, id, price, num_ppl, open_time, close_time, goodDifficulty, company);
    ASSERT_TEST(result == COMPANY_INVALID_PARAMETER);
    companyRoomRemove(company, id);

    freeCompany(company);

    return true;

}

int main (int argv, char** arc){

    RUN_TEST(testCreateCompany);
    RUN_TEST(testAddRoom);

    return 0;
}

