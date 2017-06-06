#include "test_utilities.h"
#include "list_mtm1.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

static ListElement copyString(ListElement str){
    assert(str);
    char* copy = malloc(strlen(str)+1);
    return copy != NULL ? strcpy(copy, str) : NULL;
}

static void freeString(ListElement str){
    free(str);
}

static int stringCompare(ListElement a, ListElement b)
{
    return strcmp(a, b);
}

static bool isLongerThan(ListElement element,ListFilterKey number) {
    char* string = element;
    return strlen(string) > *(int*)number;
}

static bool testListCreate() {
    ASSERT_TEST(listCreate(NULL,NULL) == NULL);
    ASSERT_TEST(listCreate(NULL,NULL) == NULL);
    ASSERT_TEST(listCreate(copyString,NULL) == NULL);
    ASSERT_TEST(listCreate(NULL,freeString) == NULL);

    return true;
}

static bool testListFilter() {
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    for (int i=0;i <5; ++i){
        listInsertFirst(list,a[i]);
    }
    int key = 5;
    List filtered = listFilter(list,isLongerThan, &key);
    ASSERT_TEST(listGetSize(filtered) == 1);
    ASSERT_TEST(strcmp(listGetFirst(filtered),a[3])==0);
    listDestroy(list);
    listDestroy(filtered);

    return true;
}

static bool testListCopy() {
    return true;
}

static bool testListGetSize() {
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    for (int i=0;i <5; ++i){
        listInsertFirst(list,a[i]);
    }
    ASSERT_TEST(listGetSize(list) == 5);
    listDestroy(list);
    return true;
}

static bool testListGetFirst() {
    char *a[6] = {"abcdefghi", "abcde", "abcdefg", "abcd", "abcdefgh", "abcdefghijk"};

    List list = listCreate(copyString,freeString);
    for (int i=0; i<6; ++i){
        listInsertFirst(list,a[i]);
    }
    ASSERT_TEST(strcmp(listGetFirst(list), "abcdefghijk") == 0);
    listDestroy(list);

    return true;
}

static bool testListGetNext() {
    char *a[6] = {"abcdefghi", "abcde", "abcdefg", "abcd", "abcdefgh", "abcdefghijk"};

    List list = listCreate(copyString,freeString);
    for (int i=0; i<6; ++i){
        listInsertFirst(list,a[i]);
    }
    listGetFirst(list);
    ASSERT_TEST(strcmp(listGetNext(list), "abcdefgh") == 0);
    while(listGetNext(list) != NULL);
    ASSERT_TEST(strcmp(listGetCurrent(list), "abcdefghi") == 0);
    listDestroy(list);

    return true;
}

static bool testListInsertFirst() {
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    for (int i=0;i <5; ++i){
        listInsertFirst(list,a[i]);
    }
    ASSERT_TEST(strcmp(listGetFirst(list), "I") == 0);
    listDestroy(list);

    return true;
}

static bool testListInsertLast() {

    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    for (int i=0;i <5; ++i){
        listInsertLast(list,a[i]);
    }
    ASSERT_TEST(strcmp(listGetFirst(list), "aaa") == 0);
    listDestroy(list);

    list = listCreate(copyString,freeString);
    listInsertLast(list,a[0]);
    ASSERT_TEST(strcmp(listGetFirst(list), "aaa") == 0);
    listDestroy(list);


    return true;
}

static bool testListInsertBeforeCurrent() {
    char *a[8] = {"str5", "str2", "str6", "str1", "str7", "str3", "str4", "str8"};

    List list = listCreate(copyString,freeString);
    for (int i=0; i<8; ++i){
        listInsertFirst(list,a[i]);
    }
    ASSERT_TEST(listGetSize())
    return true;
}

static bool testListInsertAfterCurrent() {
    return true;
}

static bool testListRemoveCurrent() {
    char *a[6] = {"abcdefghi", "abcde", "abcdefg", "abcd", "abcdefgh", "abcdefghijk"};

    List list = listCreate(copyString,freeString);
    for (int i=0; i<6; ++i){
        listInsertFirst(list,a[i]);
    }
    ASSERT_TEST(listSort(list, stringCompare) == LIST_SUCCESS);
    ASSERT_TEST(strcmp(listGetFirst(list), "abcd") == 0);
    listDestroy(list);

    return true;
}

static bool testListSort() {
    char *a[8] = {"str5", "str2", "str6", "str1", "str7", "str3", "str4", "str8"};

    List list = listCreate(copyString,freeString);
    for (int i=0; i<8; ++i){
        listInsertFirst(list,a[i]);
    }
    ASSERT_TEST(listSort(list, stringCompare) == LIST_SUCCESS);
    ASSERT_TEST(strcmp(listGetFirst(list), "str1") == 0);
    ASSERT_TEST(strcmp(listGetNext(list), "str2") == 0);
    ASSERT_TEST(strcmp(listGetNext(list), "str3") == 0);
    ASSERT_TEST(strcmp(listGetNext(list), "str4") == 0);
    ASSERT_TEST(strcmp(listGetNext(list), "str5") == 0);
    ASSERT_TEST(strcmp(listGetNext(list), "str6") == 0);
    ASSERT_TEST(strcmp(listGetNext(list), "str7") == 0);
    ASSERT_TEST(strcmp(listGetNext(list), "str8") == 0);
    ASSERT_TEST(strcmp(listGetNext(list), "str8") == 0);

    listDestroy(list);

    return true;
}

static bool testListSort2() {
    char *a[1] = {"abcdefghi"};

    List list = listCreate(copyString,freeString);
    for (int i=0; i<1; ++i){
        listInsertFirst(list,a[i]);
    }
    ASSERT_TEST(listSort(list, stringCompare) == LIST_SUCCESS);
    ASSERT_TEST(strcmp(listGetFirst(list), "abcdefghi") == 0);
    listDestroy(list);

    return true;
}
static bool testListClear() {
    char *a[7] = {"abcdefghi", "abcde", "abcdefg", "abcd", "abcdefgh", "abcdefghijk", "abcdefghijklm"};

    List list = listCreate(copyString, freeString);
    for (int i=0; i<7; ++i){
        listInsertFirst(list,a[i]);
    }
    ASSERT_TEST(listGetSize(list) == 7);
    ASSERT_TEST(listClear(list) == 0);
    ASSERT_TEST(listGetSize(list) == 0);

    for (int i=0; i<7; ++i){
        listInsertFirst(list, a[i]);
    }
    ASSERT_TEST(listSort(list, stringCompare) == LIST_SUCCESS);
    ASSERT_TEST(strcmp(listGetFirst(list), "abcd") == 0);
    ASSERT_TEST(strcmp(listGetNext(list), "abcde") == 0);
    ASSERT_TEST(strcmp(listGetNext(list), "abcdefg") == 0);
    ASSERT_TEST(strcmp(listGetNext(list), "abcdefgh") == 0);
    ASSERT_TEST(strcmp(listGetNext(list), "abcdefghi") == 0);
    ASSERT_TEST(strcmp(listGetNext(list), "abcdefghijk") == 0);
    ASSERT_TEST(strcmp(listGetNext(list), "abcdefghijklm") == 0);
    ASSERT_TEST(strcmp(listGetNext(list), "abcdefghijklm") == 0);
    listDestroy(list);

    return true;
}

static bool testListDestroy() {
    return true;
}

int main (int argv, char** arc){
    RUN_TEST(testListCreate);
    RUN_TEST(testListFilter);
    RUN_TEST(testListCopy);
    RUN_TEST(testListGetSize);
    RUN_TEST(testListGetFirst);
    RUN_TEST(testListGetNext);
    RUN_TEST(testListInsertFirst);
    RUN_TEST(testListInsertLast);
    RUN_TEST(testListInsertBeforeCurrent);
    RUN_TEST(testListInsertAfterCurrent);
    RUN_TEST(testListRemoveCurrent);
    RUN_TEST(testListSort);
    RUN_TEST(testListSort2);
    RUN_TEST(testListClear);
    RUN_TEST(testListDestroy);

    return 0;
}

