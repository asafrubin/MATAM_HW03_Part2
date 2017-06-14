#include "test_utilities.h"
#include "mtm_ex3.h"
#include "Escapers.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

static bool testcreateEscaper()
{
    char *name = "NatetheGreat@";
    char *bad_name = "Nate@theGreat@";
    TechnionFaculty faculty = ELECTRICAL_ENGINEERING;
    int skill = 10 , bad_skill = -7;


    EscaperResult result;
    Escaper escaper = NULL;

    escaper = createEscaper(name , faculty , skill , &result);
    ASSERT_TEST(escaper != NULL);
    ASSERT_TEST(result == ESCAPER_SUCCESS);
    freeEscaper(escaper);

    escaper = createEscaper(bad_name , faculty , skill , &result);
    ASSERT_TEST(escaper == NULL);
    ASSERT_TEST(result == ESCAPER_INVALID_PARAMETER);
    freeEscaper(escaper);

    escaper = createEscaper(name , faculty , bad_skill , &result);
    ASSERT_TEST(escaper == NULL);
    ASSERT_TEST(result == ESCAPER_INVALID_PARAMETER);
    freeEscaper(escaper);

    return true;
}


static bool testescaperCopyElement()
{
    char *name = "NatetheGreat@";
    char *testmail = NULL;
    TechnionFaculty faculty = ELECTRICAL_ENGINEERING;
    TechnionFaculty test_faculty = UNKNOWN;
    int skill = 10;
    int test_skill = -1;

    EscaperResult result;
    Escaper escaper = NULL;
    Escaper copy_escaper = NULL;

    escaper = createEscaper(name , faculty , skill , &result);
    ASSERT_TEST(escaper != NULL);
    ASSERT_TEST(result == ESCAPER_SUCCESS);

    copy_escaper = escaperCopyElement(escaper);
    test_skill = escaperGetSkill(copy_escaper);
    ASSERT_TEST(test_skill == skill);
    testmail = escaperGetEmail(copy_escaper , &result);
    ASSERT_TEST(result == ESCAPER_SUCCESS);
    ASSERT_TEST(strcmp(testmail , name) == 0);

    test_faculty = escaperGetFaculty(copy_escaper);
    ASSERT_TEST(test_faculty == faculty);

    freeEscaper(escaper);
    freeEscaper(copy_escaper);

    return true;
}

int escaperCompare(Escaper escaper1, Escaper escaper2);
static bool testescaperCompare()
{
    char *name = "NatetheGreat@";
    char *testmail = "Natethe@notsogreat";
    TechnionFaculty faculty = ELECTRICAL_ENGINEERING;
    int skill = 10;
    int test = 100;

    EscaperResult result;
    Escaper escaper = NULL;
    Escaper escaper2 = NULL;
    Escaper copy_escaper = NULL;

    escaper = createEscaper(name , faculty , skill , &result);
    ASSERT_TEST(escaper != NULL);
    ASSERT_TEST(result == ESCAPER_SUCCESS);

    escaper2 = createEscaper(testmail , faculty , skill , &result);
    ASSERT_TEST(escaper != NULL);
    ASSERT_TEST(result == ESCAPER_SUCCESS);

    copy_escaper = escaperCopyElement(escaper);

    test = escaperCompare(escaper , copy_escaper);
    ASSERT_TEST(test == 0);

    test = escaperCompare(escaper , escaper2);
    ASSERT_TEST(test != 0);

    freeEscaper(escaper);
    freeEscaper(escaper2);
    freeEscaper(copy_escaper);

    return true;
}


int main (int argv, char** arc){

    RUN_TEST(testcreateEscaper);
    RUN_TEST(testescaperCopyElement);
    RUN_TEST(testescaperCompare);

    return 0;
}

