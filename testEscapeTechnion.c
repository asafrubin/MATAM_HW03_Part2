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

    //comapny add all good parameters
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
    mtmErrorCode = mtmRoomAdd(goodCompanyEmail1, 25, price, numOfPpl, openHour, closeHour, difficulty, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmEscaperAdd("good@email", ELECTRICAL_ENGINEERING, 9, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmEscaperOrder("good@email", companyFaculty, 25, 0, 0, 4, escapeTechnion);
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
    //remove room with existing order
    mtmErrorCode = mtmRoomRemove(companyFaculty, 25, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_RESERVATION_EXISTS);

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
    char *goodCompanyEmail1 = "asaf@gmail", *notExistCompanyEmail = "asaf2@gmail";
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

    //escaper remove with email that does not exist
    mtmErrorCode = mtmEscaperRemove(notExistCompanyEmail, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_CLIENT_EMAIL_DOES_NOT_EXIST);

    //escaper remove with NULL email parameter
    mtmErrorCode = mtmEscaperRemove(NULL, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_INVALID_PARAMETER);

    mtmDestroyEscapeTechnion(escapeTechnion);
    return true;

}

static bool testMtmEscaperOrder()
{
    EscapeTechnion escapeTechnion = NULL;
    FILE *outputStream = stdout;
    char *goodCompanyEmail1 = "asaf@gmail", *goodCompanyEmail2 = "asaf2@gmail";
    char *goodEscaperEmail1 = "nate@gmail", *notExistEscaperEmail = "a@a";// , *badEmail1 = "nate@@";
    char *goodEscaperEmail2 = "Bro@";
    TechnionFaculty escaperFaculty1 = COMPUTER_SCIENCE, escaperFaculty2 = ELECTRICAL_ENGINEERING,
            companyFaculty1 = COMPUTER_SCIENCE, companyFaculty2 = BIOLOGY;
    MtmErrorCode mtmErrorCode;
    int goodSkill = 5;
    int id1 = 5, id2 = 6, price = 20, numOfPpl = 4, openHour = 12, closeHour = 16, difficulty = 9;

    escapeTechnion = mtmCreateEscapeTechnion(outputStream);
    ASSERT_TEST( escapeTechnion != NULL );
    mtmErrorCode = mtmCompanyAdd(goodCompanyEmail1, companyFaculty1, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmCompanyAdd(goodCompanyEmail2, companyFaculty2, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmRoomAdd(goodCompanyEmail1, id1, price, numOfPpl, openHour, closeHour, difficulty, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmRoomAdd(goodCompanyEmail2, id2, price, numOfPpl, openHour, closeHour, difficulty, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmEscaperAdd(goodEscaperEmail1, escaperFaculty1, goodSkill, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmEscaperAdd(goodEscaperEmail2, escaperFaculty2, goodSkill, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);

    //start test escaperOrder()
    //good parameters
    mtmErrorCode = mtmEscaperOrder(goodEscaperEmail1, companyFaculty2, id2, 0, 0, 5, escapeTechnion );
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmEscaperOrder(goodEscaperEmail1, companyFaculty1, id1, 1, 0, 5, escapeTechnion );
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmEscaperOrder(goodEscaperEmail2, companyFaculty2, id2, 1, 0, 5, escapeTechnion );
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmEscaperOrder(goodEscaperEmail2, companyFaculty1, id1, 0, 0, 5, escapeTechnion );
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    //check Order Collide
    mtmErrorCode = mtmEscaperOrder(goodEscaperEmail1, companyFaculty2, id2, 0, 0, 5, escapeTechnion );
    ASSERT_TEST(mtmErrorCode == MTM_CLIENT_IN_ROOM);

    /**************check Error Sequence*************/

    // email not exist and bad start time
    mtmErrorCode = mtmEscaperOrder(notExistEscaperEmail, companyFaculty2, id2, 23, 0, 5, escapeTechnion );
    ASSERT_TEST(mtmErrorCode == MTM_INVALID_PARAMETER);
    //check Error Sequence: good email but not exist, and bad room id
    mtmErrorCode = mtmEscaperOrder(notExistEscaperEmail, companyFaculty2, 25, 9, 0, 5, escapeTechnion );
    ASSERT_TEST(mtmErrorCode == MTM_CLIENT_EMAIL_DOES_NOT_EXIST);
    //check Error Sequence: order collide, and bad room id
    mtmErrorCode = mtmEscaperOrder(goodEscaperEmail1, companyFaculty2, 25, 0, 0, 5, escapeTechnion );
    ASSERT_TEST(mtmErrorCode == MTM_ID_DOES_NOT_EXIST);
    //check Error Sequence: client in room, and room not available
    mtmErrorCode = mtmEscaperOrder(goodEscaperEmail2, companyFaculty1, id1, 1, 0, 5, escapeTechnion );
    ASSERT_TEST(mtmErrorCode == MTM_CLIENT_IN_ROOM);

    //need to create test for order discount







    mtmDestroyEscapeTechnion(escapeTechnion);
    return true;

}

static bool testMtmEscaperRecommend()
{
    EscapeTechnion escapeTechnion = NULL;
    FILE *outputStream = stdout;
    char *goodCompanyEmail1 = "company1@gmail", *goodCompanyEmail2 = "company2@gmail";
    char *goodEscaperEmail1 = "nate@gmail", *badEscaperEmail1 = "nate@@", *notExistEscaperEmail = "a@a";
    TechnionFaculty escaperFaculty = COMPUTER_SCIENCE, companyFaculty1 = COMPUTER_SCIENCE, companyFaculty2 = BIOLOGY;
    MtmErrorCode mtmErrorCode;
    int goodSkill = 5;
    int id1 = 5, id2 = 6, price = 20, numOfPpl = 4, openHour = 12, closeHour = 16, difficulty1 = 9, difficulty2 = 5;

    escapeTechnion = mtmCreateEscapeTechnion(outputStream);
    ASSERT_TEST( escapeTechnion != NULL );
    mtmErrorCode = mtmCompanyAdd(goodCompanyEmail1, companyFaculty1, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmCompanyAdd(goodCompanyEmail2, companyFaculty2, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmRoomAdd(goodCompanyEmail1, id1, price, numOfPpl, openHour, closeHour, difficulty1, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmRoomAdd(goodCompanyEmail2, id2, 40, numOfPpl, openHour, closeHour, difficulty2, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmEscaperAdd(goodEscaperEmail1, escaperFaculty, goodSkill, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);

    //start test mtmEscaperRecommend()
    //escaper recommend good parameters
    mtmErrorCode = mtmEscaperRecommend(goodEscaperEmail1, 5, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmEscaperOrder(goodEscaperEmail1, companyFaculty2,id2, 0, 0, 5,escapeTechnion );
    ASSERT_TEST(mtmErrorCode == MTM_CLIENT_IN_ROOM);
    //escaper recommend bad email
    mtmErrorCode = mtmEscaperRecommend(badEscaperEmail1, 5, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_INVALID_PARAMETER);
    //escaper recommend email not exist
    mtmErrorCode = mtmEscaperRecommend(notExistEscaperEmail, 5, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_CLIENT_EMAIL_DOES_NOT_EXIST);


    //need to create test for escaper recommended room id distance !!!

    mtmDestroyEscapeTechnion(escapeTechnion);
    return true;
}

static bool testMtmReportDay()
{
    EscapeTechnion escapeTechnion = NULL;
    FILE *outputStream = stdout;
    char *goodCompanyEmail1 = "asaf@gmail", *goodCompanyEmail2 = "asaf2@gmail";
    char *goodEscaperEmail1 = "nate@gmail";
    char *goodEscaperEmail2 = "Bro@";
    TechnionFaculty escaperFaculty1 = COMPUTER_SCIENCE, escaperFaculty2 = ELECTRICAL_ENGINEERING,
            companyFaculty1 = COMPUTER_SCIENCE, companyFaculty2 = BIOLOGY;
    MtmErrorCode mtmErrorCode;
    int goodSkill = 5;
    int id1 = 5, id2 = 6, price = 20, numOfPpl = 4, openHour = 12, closeHour = 16, difficulty = 9;

    escapeTechnion = mtmCreateEscapeTechnion(outputStream);
    ASSERT_TEST( escapeTechnion != NULL );
    mtmErrorCode = mtmCompanyAdd(goodCompanyEmail1, companyFaculty1, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmCompanyAdd(goodCompanyEmail2, companyFaculty2, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmRoomAdd(goodCompanyEmail1, id1, price, numOfPpl, openHour, closeHour, difficulty, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmRoomAdd(goodCompanyEmail2, id2, price, numOfPpl, openHour, closeHour, difficulty, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmEscaperAdd(goodEscaperEmail1, escaperFaculty1, goodSkill, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmEscaperAdd(goodEscaperEmail2, escaperFaculty2, goodSkill, escapeTechnion);
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);

    //start test escaperOrder()
    //good parameters
    mtmErrorCode = mtmEscaperOrder(goodEscaperEmail1, companyFaculty2, id2, 0, 0, 5, escapeTechnion );
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmEscaperOrder(goodEscaperEmail1, companyFaculty1, id1, 1, 0, 5, escapeTechnion );
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmEscaperOrder(goodEscaperEmail2, companyFaculty2, id2, 1, 0, 5, escapeTechnion );
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);
    mtmErrorCode = mtmEscaperOrder(goodEscaperEmail2, companyFaculty1, id1, 0, 0, 5, escapeTechnion );
    ASSERT_TEST(mtmErrorCode == MTM_SUCCESS);

    mtmReportDay(escapeTechnion);
    mtmReportDay(escapeTechnion);
    mtmReportDay(escapeTechnion);

    mtmReportBest(escapeTechnion);
    mtmReportBest(escapeTechnion);


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
    RUN_TEST(testMtmEscaperOrder);
    RUN_TEST(testMtmEscaperRecommend);
    RUN_TEST(testMtmReportDay);

    return 0;
}