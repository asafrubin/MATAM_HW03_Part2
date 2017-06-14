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
    TechnionFaculty badfaculty = (TechnionFaculty)-1;

    char    *goodEmail = "asaf@gmail.com",
            *badEmail1 = "asaf@@gmail.com",
            *badEmail2 = "asaf";

    newCompany = createCompany(goodEmail,faculty, &companyResult);
    ASSERT_TEST(companyResult == COMPANY_SUCCESS);
    freeCompany(newCompany);

    newCompany = createCompany(badEmail1,faculty, &companyResult);
    ASSERT_TEST(companyResult == COMPANY_INVALID_PARAMETER);
    freeCompany(newCompany);

    newCompany = createCompany(badEmail2,faculty, &companyResult);
    ASSERT_TEST(companyResult == COMPANY_INVALID_PARAMETER);
    freeCompany(newCompany);

    newCompany = createCompany(goodEmail, badfaculty, &companyResult);
    ASSERT_TEST(companyResult == COMPANY_INVALID_PARAMETER);
    freeCompany(newCompany);

    return true;
}

static bool testAddRoom()
{
    char *goodEmail = "asaf@gmail.com";
    char *badEmail1 = "nate@@gmail.com";
    char *badEmail2 = "BadEmail";

    int id = 5, id2 = 4, price = 4*15, num_ppl = 4, open_time = 12, close_time = 16, goodDifficulty = 8, badDifficulty = 12,
        bad_price = 7, bad_open_time = -17, bad_close_time = 25, bad_num_ppl = -1;
    CompanyResult result;
    Company company;
    Company company2;

    company = createCompany(goodEmail, COMPUTER_SCIENCE, &result);
    ASSERT_TEST(result == COMPANY_SUCCESS);

    company2 = createCompany(goodEmail, COMPUTER_SCIENCE, &result);
    ASSERT_TEST(result == COMPANY_SUCCESS);

    //tests each paramater
    result = addCompanyRoom(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty, company);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    companyRoomRemove(company, id);
    result = addCompanyRoom(badEmail1, id, price, num_ppl, open_time, close_time, goodDifficulty, company);
    ASSERT_TEST(result == COMPANY_INVALID_PARAMETER);
    companyRoomRemove(company, id);
    result = addCompanyRoom(goodEmail, id, price, num_ppl, open_time, close_time, badDifficulty, company);
    ASSERT_TEST(result == COMPANY_INVALID_PARAMETER);
    companyRoomRemove(company, id);
    result = addCompanyRoom(badEmail2, id, price, num_ppl, open_time, close_time, goodDifficulty, company2);
    ASSERT_TEST(result == COMPANY_INVALID_PARAMETER);
    companyRoomRemove(company2, id);
    result = addCompanyRoom(goodEmail, id, bad_price, num_ppl, open_time, close_time, goodDifficulty, company2);
    ASSERT_TEST(result == COMPANY_INVALID_PARAMETER);
    companyRoomRemove(company2, id);
    result = addCompanyRoom(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty, company2);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    companyRoomRemove(company2, id);
    result = addCompanyRoom(goodEmail, id, price, num_ppl, bad_open_time, close_time, goodDifficulty, company2);
    ASSERT_TEST(result == COMPANY_INVALID_PARAMETER);
    companyRoomRemove(company2, id);
    result = addCompanyRoom(goodEmail, id, price, num_ppl, open_time, bad_close_time, goodDifficulty, company2);
    ASSERT_TEST(result == COMPANY_INVALID_PARAMETER);
    companyRoomRemove(company2, id);
    result = addCompanyRoom(goodEmail, id2, price, num_ppl, open_time, close_time, goodDifficulty, company2);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    companyRoomRemove(company2, id2);
    result = addCompanyRoom(goodEmail, id2, price, bad_num_ppl, open_time, close_time, goodDifficulty, company2);
    ASSERT_TEST(result == COMPANY_INVALID_PARAMETER);
    companyRoomRemove(company2, id2);


    //checks if room already exists
    result = addCompanyRoom(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty, company);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    result = addCompanyRoom(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty, company);
    ASSERT_TEST(result == COMPANY_ROOM_ID_ALREADY_EXIST);
    result = addCompanyRoom(badEmail1, id, price, num_ppl, open_time, close_time, goodDifficulty, company);
    ASSERT_TEST(result == COMPANY_INVALID_PARAMETER);
    companyRoomRemove(company, id);

    //sent null as company
    result = addCompanyRoom(badEmail1, id, price, num_ppl, open_time, close_time, goodDifficulty, NULL);
    ASSERT_TEST(result == COMPANY_NULL_PARAMETER);
    companyRoomRemove(company, id);



    freeCompany(company);
    freeCompany(company2);

    return true;

}

static bool testgetCompanyRoomDifficultyById()
{
    char *goodEmail = "asaf@gmail.com";

    int id = 5, price = 4*15, num_ppl = 4, open_time = 12, close_time = 16, goodDifficulty = 8 , check = 0;
    CompanyResult result;
    Company company;

    company = createCompany(goodEmail, COMPUTER_SCIENCE, &result);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    result = addCompanyRoom(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty, company);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    check = getCompanyRoomDifficultyById(company , id);
    ASSERT_TEST(check == goodDifficulty);
    freeCompany(company);

    return true;
}

static bool testgetCompanyRoomPriceById()
{
    char *goodEmail = "asaf@gmail.com";

    int id = 5, price = 4*15, num_ppl = 4, open_time = 12, close_time = 16, goodDifficulty = 8 , check = 0;
    CompanyResult result;
    Company company;

    company = createCompany(goodEmail, COMPUTER_SCIENCE, &result);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    result = addCompanyRoom(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty, company);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    check = getCompanyRoomPriceById(company , id);
    ASSERT_TEST(check == price);
    freeCompany(company);

    return true;
}

static bool testgetCompanyRoomNumOfPplById()
{
    char *goodEmail = "asaf@gmail.com";

    int id = 5, price = 4*15, num_ppl = 4, open_time = 12, close_time = 16, goodDifficulty = 8 , check = 0 , bad_id = 0;
    CompanyResult result;
    Company company;

    company = createCompany(goodEmail, COMPUTER_SCIENCE, &result);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    result = addCompanyRoom(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty, company);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    check = getCompanyRoomNumOfPplById(company , id);
    ASSERT_TEST(check == num_ppl);
    check = getCompanyRoomNumOfPplById(company , bad_id);
    ASSERT_TEST(check == -1);

    freeCompany(company);

    return true;
}


static bool testcheckIfRoomExists()
{
    char *goodEmail = "asaf@gmail.com";

    int id = 5, id2 = 4, price = 4*15, num_ppl = 4, open_time = 12, close_time = 16, goodDifficulty = 8;
    CompanyResult result;
    Company company;

    company = createCompany(goodEmail, COMPUTER_SCIENCE, &result);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    result = addCompanyRoom(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty, company);
    ASSERT_TEST(result == COMPANY_SUCCESS);

    result = checkIfRoomExists(company , id);
    ASSERT_TEST(result == COMPANY_ROOM_ID_ALREADY_EXIST);
    result = checkIfRoomExists(company , id2);
    ASSERT_TEST(result == COMPANY_ROOM_ID_DOES_NOT_EXIST);

    freeCompany(company);

    return true;
}

int compareCompanies(Company firstCompany , Company secondCompany);
static bool testcompareCompanies()
{
    char *goodEmail1 = "asaf@gmail.com";
    char *goodEmail2 = "nate@gmail.com";


    int test = 0;
    CompanyResult result;
    Company company;
    Company company2;
    Company company3;

    company = createCompany(goodEmail1, ELECTRICAL_ENGINEERING, &result);
    ASSERT_TEST(result == COMPANY_SUCCESS);

    company2 = createCompany(goodEmail2, COMPUTER_SCIENCE, &result);
    ASSERT_TEST(result == COMPANY_SUCCESS);

    company3 = createCompany(goodEmail2, COMPUTER_SCIENCE, &result);
    ASSERT_TEST(result == COMPANY_SUCCESS);

    test = compareCompanies(company2 , company3);
    ASSERT_TEST(test == 0);

    test = compareCompanies(company , company2);
    ASSERT_TEST(test < 0);

    test = compareCompanies(company2 , company);
    ASSERT_TEST(test > 0);

    freeCompany(company);
    freeCompany(company2);
    freeCompany(company3);

    return true;
}


static bool testgetCompanyRoomListSize() {
    char *goodEmail = "asaf@gmail.com";

    int id = 5, id2 = 4, price = 4 * 15, num_ppl = 4, open_time = 12, close_time = 16, goodDifficulty = 8 , test = 0;
    CompanyResult result;
    Company company;
    Company company2;

    company = createCompany(goodEmail, COMPUTER_SCIENCE, &result);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    result = addCompanyRoom(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty, company);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    result = addCompanyRoom(goodEmail, id2, price, num_ppl, open_time, close_time, goodDifficulty, company);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    test = getCompanyRoomListSize(company);
    ASSERT_TEST(test == 2);

    test = getCompanyRoomListSize(NULL);
    ASSERT_TEST(test == -1);

    company2 = createCompany(goodEmail, COMPUTER_SCIENCE, &result);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    test = getCompanyRoomListSize(company2);
    ASSERT_TEST(test == 0);

    freeCompany(company);
    freeCompany(company2);

    return true;
}


static bool testgetCompanyRecommendedRoomId() {
    char *goodEmail = "asaf@gmail.com";

    int id = 5, price = 4 * 15, num_ppl = 5, open_time = 12, close_time = 16, goodDifficulty = 8, test = 0;
    int id2 = 4, num_ppl2 = 1, goodDifficulty2 = 4;
    int id3 = 3, num_ppl3 = 1, goodDifficulty3 = 4;
    int P_e = 5, skill_level = 8;
    int P_e2 = 1, skill_level2 = 3;
    int P_e3 = 3, skill_level3 = 6;
    double best_calculation = 999999;
    CompanyResult result;
    Company company;

    company = createCompany(goodEmail, COMPUTER_SCIENCE, &result);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    result = addCompanyRoom(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty, company);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    result = addCompanyRoom(goodEmail, id2, price, num_ppl2, open_time, close_time, goodDifficulty2, company);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    result = addCompanyRoom(goodEmail, id3, price, num_ppl3, open_time, close_time, goodDifficulty3, company);
    ASSERT_TEST(result == COMPANY_SUCCESS);

    test = getCompanyRecommendedRoomId(company, P_e, skill_level, &best_calculation);
    ASSERT_TEST(test == 5);
    ASSERT_TEST(best_calculation == 0);
    best_calculation = 999999;

    test = getCompanyRecommendedRoomId(company, P_e2, skill_level2, &best_calculation);
    ASSERT_TEST(test == 3);
    ASSERT_TEST(best_calculation == 1);
    best_calculation = 999999;

    test = getCompanyRecommendedRoomId(company, P_e3, skill_level3, &best_calculation);
    ASSERT_TEST(test == 3);
    ASSERT_TEST(best_calculation == 8);

    freeCompany(company);

    return true;
}

CompanyResult mtmCheckCompanyRoomParameters(char *companyEmail, int roomId, int roomPrice, int num_ppl, int open_time,
                                            int close_time, int diffuculty);
static bool testmtmCheckCompanyRoomParameters() {
    char *goodEmail = "asaf@gmail.com";

    int id = 5, price = 4 * 15, num_ppl = 5, open_time = 12, close_time = 16, goodDifficulty = 8;
    int id2 = 4, price2 = 1 * 15, num_ppl2 = 0, open_time2 = 24, close_time2 = 0, badDifficulty = -1;

    CompanyResult result;

    result = mtmCheckCompanyRoomParameters(goodEmail, id, price, num_ppl, open_time, close_time, goodDifficulty);
    ASSERT_TEST(result == COMPANY_SUCCESS);

    result = mtmCheckCompanyRoomParameters(goodEmail, id2, price, num_ppl, open_time, close_time, badDifficulty);
    ASSERT_TEST(result == COMPANY_INVALID_PARAMETER);
    result = mtmCheckCompanyRoomParameters(goodEmail, id2, price2, num_ppl, open_time, close_time, goodDifficulty);
    ASSERT_TEST(result == COMPANY_INVALID_PARAMETER);
    result = mtmCheckCompanyRoomParameters(goodEmail, id2, price, num_ppl2, open_time, close_time, goodDifficulty);
    ASSERT_TEST(result == COMPANY_INVALID_PARAMETER);
    result = mtmCheckCompanyRoomParameters(goodEmail, id2, price, num_ppl, open_time2, close_time, goodDifficulty);
    ASSERT_TEST(result == COMPANY_INVALID_PARAMETER);
    result = mtmCheckCompanyRoomParameters(goodEmail, id2, price, num_ppl, open_time, close_time2, goodDifficulty);
    ASSERT_TEST(result == COMPANY_INVALID_PARAMETER);

    return true;
}


static bool testgetCompanyEmail() {
    char *goodEmail = "asaf@gmail.com";
    char *str = NULL;

    CompanyResult result;
    Company company;

    company = createCompany(goodEmail, COMPUTER_SCIENCE, &result);
    ASSERT_TEST(result == COMPANY_SUCCESS);

    str = getCompanyEmail(company, &result);
    ASSERT_TEST(strcmp(str , goodEmail) == 0);
    ASSERT_TEST(result == COMPANY_SUCCESS);

    freeCompany(company);

    return true;
}

static bool testgetCompanyFaculty() {
    char *goodEmail = "asaf@gmail.com";

    CompanyResult result;
    Company company;
    TechnionFaculty test = UNKNOWN;

    company = createCompany(goodEmail, COMPUTER_SCIENCE, &result);
    ASSERT_TEST(result == COMPANY_SUCCESS);

    result = getCompanyFaculty(company, &test);
    ASSERT_TEST(result == COMPANY_SUCCESS);
    ASSERT_TEST(test == COMPUTER_SCIENCE);

    freeCompany(company);

    return true;
}

int main (int argv, char** arc){

    RUN_TEST(testCreateCompany);
    RUN_TEST(testAddRoom);
    RUN_TEST(testgetCompanyRoomDifficultyById);
    RUN_TEST(testgetCompanyRoomPriceById);
    RUN_TEST(testgetCompanyRoomNumOfPplById);
    RUN_TEST(testcheckIfRoomExists);
    RUN_TEST(testcompareCompanies);
    RUN_TEST(testgetCompanyRoomListSize);
    RUN_TEST(testgetCompanyRecommendedRoomId);
    RUN_TEST(testmtmCheckCompanyRoomParameters);
    RUN_TEST(testgetCompanyEmail);
    RUN_TEST(testgetCompanyFaculty);


    return 0;
}

//