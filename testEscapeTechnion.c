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

static bool testCreateEscapeTechnion()
{
    EscapeTechnion escapeTechnion = NULL;
    //FILE *inputStream = stdin;
    FILE *outputStream = stdout;

    escapeTechnion = mtmCreateEscapeTechnion(outputStream);
    ASSERT_TEST( escapeTechnion != NULL );
    mtmDestroyEscapeTechnion(escapeTechnion);

    return true;
}

static bool testMtmCompanyAdd()
{
    EscapeTechnion escapeTechnion = NULL;
    FILE *outputStream = stdout;
    char *goodCompanyEmail = "asaf@gmail";
    TechnionFaculty companyFaculty = COMPUTER_SCIENCE;
    MtmErrorCode mtmErrorCode;

    escapeTechnion = mtmCreateEscapeTechnion(outputStream);
    ASSERT_TEST( escapeTechnion != NULL );
    mtmErrorCode = mtmCompanyAdd(goodCompanyEmail, companyFaculty, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);

    mtmDestroyEscapeTechnion(escapeTechnion);

    return true;
}

static bool testMtmCompanyRemove()
{
    EscapeTechnion escapeTechnion = NULL;
    FILE *outputStream = stdout;
    char *goodCompanyEmail = "asaf@gmail";
    TechnionFaculty companyFaculty = COMPUTER_SCIENCE;
    MtmErrorCode mtmErrorCode;

    escapeTechnion = mtmCreateEscapeTechnion(outputStream);
    ASSERT_TEST( escapeTechnion != NULL );
    mtmErrorCode = mtmCompanyAdd(goodCompanyEmail, companyFaculty, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmCompanyRemove(goodCompanyEmail, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmDestroyEscapeTechnion(escapeTechnion);

    return true;
}

static bool testMtmRoomAdd()
{
    EscapeTechnion escapeTechnion = NULL;
    FILE *outputStream = stdout;
    char *goodCompanyEmail1 = "asaf@gmail", *badRoomEmail = "asaf@";
    TechnionFaculty companyFaculty = COMPUTER_SCIENCE;
    MtmErrorCode mtmErrorCode;
    int id1 = 5, id2 = 6, price = 20, numOfPpl = 4, openHour = 12, closeHour = 16, difficulty = 9, badOpenHour = 17;

    //contructing EscapeTechnion and Company
    escapeTechnion = mtmCreateEscapeTechnion(outputStream);
    ASSERT_TEST( escapeTechnion != NULL );
    mtmErrorCode = mtmCompanyAdd(goodCompanyEmail1, companyFaculty, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    /*Start testing the RoomAdd*/
    //All good parameters
    mtmErrorCode = mtmRoomAdd(goodCompanyEmail1, id1, price, numOfPpl, openHour, closeHour, difficulty, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    //trying add room with id already exist
    mtmErrorCode = mtmRoomAdd(goodCompanyEmail1, id1, price, numOfPpl, openHour, closeHour, difficulty, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_ID_ALREADY_EXIST);
    //add room with email that does not exist
    mtmErrorCode = mtmRoomAdd(badRoomEmail, id1, price, numOfPpl, openHour, closeHour, difficulty, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_COMPANY_EMAIL_DOES_NOT_EXIST);
    //add room with openHour > closeHour
    mtmErrorCode = mtmRoomAdd(goodCompanyEmail1, id2, price, numOfPpl, badOpenHour, closeHour, difficulty, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_INVALID_PARAMETER);

    //destroy EscapeTechnion
    mtmDestroyEscapeTechnion(escapeTechnion);

    return true;
}

static bool testMtmRoomRemove()
{
    EscapeTechnion escapeTechnion = NULL;
    FILE *outputStream = stdout;
    char *goodCompanyEmail1 = "asaf@gmail";
    TechnionFaculty companyFaculty = COMPUTER_SCIENCE, badCompanyFaculty = BIOLOGY;
    MtmErrorCode mtmErrorCode;
    int id1 = 5, id2 = 6, price = 20, numOfPpl = 4, openHour = 12, closeHour = 16, difficulty = 9;

    //contructing EscapeTechnion and Company
    escapeTechnion = mtmCreateEscapeTechnion(outputStream);
    ASSERT_TEST( escapeTechnion != NULL );
    mtmErrorCode = mtmCompanyAdd(goodCompanyEmail1, companyFaculty, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    //adding room
    mtmErrorCode = mtmRoomAdd(goodCompanyEmail1, id1, price, numOfPpl, openHour, closeHour, difficulty, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);

    /*Start testing the RoomRemove*/

    //trying remove room with bad id
    mtmErrorCode = mtmRoomRemove(companyFaculty, id2, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_ID_DOES_NOT_EXIST);
    //try to remove room with bad company faculty
    mtmErrorCode = mtmRoomRemove(badCompanyFaculty, id1, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_ID_DOES_NOT_EXIST);
    //All good parameters
    mtmErrorCode = mtmRoomRemove(companyFaculty, id1, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);

    //destroy EscapeTechnion
    mtmDestroyEscapeTechnion(escapeTechnion);

    return true;
}

static bool testMtmEscaperAdd()
{
    EscapeTechnion escapeTechnion = NULL;
    FILE *outputStream = stdout;
    char *goodCompanyEmail1 = "asaf@gmail", *goodCompanyEmail2 = "asaf2@gmail";
    TechnionFaculty escaperFaculty = COMPUTER_SCIENCE;
    MtmErrorCode mtmErrorCode;
    int goodSkill = 5, badSkill1 = 12, badSkill2 = -20;

    escapeTechnion = mtmCreateEscapeTechnion(outputStream);
    ASSERT_TEST( escapeTechnion != NULL );

    //start test mtmEscaperAdd()
    //escaper add with good parameters
    mtmErrorCode = mtmEscaperAdd(goodCompanyEmail1, escaperFaculty, goodSkill, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    //escaper add with existing email already
    mtmErrorCode = mtmEscaperAdd(goodCompanyEmail1, escaperFaculty, goodSkill, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_EMAIL_ALREADY_EXISTS);
    //escaper add with bad skill1
    mtmErrorCode = mtmEscaperAdd(goodCompanyEmail2, escaperFaculty, badSkill1, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_INVALID_PARAMETER);
    //escaper add with bad skill2
    mtmErrorCode = mtmEscaperAdd(goodCompanyEmail2, escaperFaculty, badSkill2, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_INVALID_PARAMETER);


    mtmDestroyEscapeTechnion(escapeTechnion);
    return true;
}

static bool testMtmEscaperRemove()
{
    EscapeTechnion escapeTechnion = NULL;
    FILE *outputStream = stdout;
    char *goodCompanyEmail1 = "asaf@gmail", *badCompanyEmail2 = "asaf2@gmail";
    TechnionFaculty escaperFaculty = COMPUTER_SCIENCE;
    MtmErrorCode mtmErrorCode;
    int goodSkill = 5;

    escapeTechnion = mtmCreateEscapeTechnion(outputStream);
    ASSERT_TEST( escapeTechnion != NULL );
    mtmErrorCode = mtmEscaperAdd(goodCompanyEmail1, escaperFaculty, goodSkill, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);

    //start test mtmEscaperRemove()
    //escaper remove with good parameters
    mtmErrorCode = mtmEscaperRemove(goodCompanyEmail1, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmEscaperRemove(badCompanyEmail2, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_CLIENT_EMAIL_DOES_NOT_EXIST);


    mtmDestroyEscapeTechnion(escapeTechnion);
    return true;

}


int main (int argv, char** arc)
{

    RUN_TEST(testCreateEscapeTechnion);
    RUN_TEST(testMtmCompanyAdd);
    RUN_TEST(testMtmCompanyRemove);
    RUN_TEST(testMtmRoomAdd);
    RUN_TEST(testMtmRoomRemove);
    RUN_TEST(testMtmEscaperAdd);
    RUN_TEST(testMtmEscaperRemove);

    return 0;
}

