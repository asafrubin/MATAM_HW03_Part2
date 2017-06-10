#include "test_utilities.h"
#include "mtm_ex3.h"
#include "Escapers.h"
#include "Companies.h"
#include "Orders.h"
#include "EscapeTechnion.h"
#include "set.h"
#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

static bool testCreateCompany()
{
    Company newCompany;
    char *companyEmail = "nate@gmail.com";
    TechnionFaculty companyFaculty = COMPUTER_SCIENCE;
    CompanyResult companyResult;

    newCompany = createCompany(companyEmail, companyFaculty, &companyResult);
    ASSERT_TEST(companyResult == COMPANY_SUCCESS);
    ASSERT_TEST(newCompany != NULL);

    return true;
}

static bool testCreateEscaper() {

    EscaperResult result;
    Escaper newEscaper = NULL;
    char *goodEmail = "asaf@gmail.com";
    char *badEmail1 = "asaf@@gmail.com";
    char *badEmail2 = "aaaaa";

    int goodSkill = 9, badSkill = 12;
    TechnionFaculty goodFaculty = COMPUTER_SCIENCE;

    //first test - all good
    newEscaper = createEscaper(goodEmail, goodFaculty, goodSkill, &result);
    freeEscaper(&newEscaper);
    ASSERT_TEST(result == ESCAPER_SUCCESS);

    //second test - bad email
    newEscaper = createEscaper(badEmail1, goodFaculty, goodSkill, &result);
    freeEscaper(&newEscaper);

    ASSERT_TEST(result == ESCAPER_INVALID_PARAMETER);

    //third test - bad email 2
    newEscaper = createEscaper(badEmail2, goodFaculty, goodSkill, &result);
    freeEscaper(&newEscaper);
    ASSERT_TEST(result == ESCAPER_INVALID_PARAMETER);

    //third test - bad skill
    newEscaper = createEscaper(goodEmail, goodFaculty, badSkill, &result);
    freeEscaper(&newEscaper);
    ASSERT_TEST(result == ESCAPER_INVALID_PARAMETER);

    return true;
}

static bool testCopyElement()
{

    return true;
}

int main (int argv, char** arc)
{

    RUN_TEST(testCreateEscaper);
    RUN_TEST(testCreateCompany);
    RUN_TEST(testCopyElement);

    return 0;
}

