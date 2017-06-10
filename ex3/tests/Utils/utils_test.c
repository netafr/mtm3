#include "test_utilities.h"
#include "../../Utils.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "../../mtm_ex3.h"

static void* CopyString(void* str) {
    if(str == NULL) {
        return NULL;
    }
    char* temp = (char*)str;
    char* new_str = StrDuplicate(temp);
    return new_str;
}

static void FreeString(void* str) {
    if(str == NULL) {
        return;
    }
    free((char*)str);
}

static bool ListsEqual(List list1, List list2) {
    assert(list1 != NULL && list2 != NULL);
    if(listGetSize(list1) != listGetSize(list2)) {
        return false;
    }
    char* str1 = listGetFirst(list1);
    char* str2 = listGetFirst(list2);
    
    while(str1 != NULL) {
        if(strcmp(str1, str2) != 0) {
        return false;
        }
        str1 = listGetNext(list1);
        str2 = listGetNext(list2);
    }
    return true;
}

static bool StringOccCheck() {
    ASSERT_TEST(StringOccurencesOfChar("hi", '@') == 0);
    ASSERT_TEST(StringOccurencesOfChar("@fsdf", '@') == 1);
    ASSERT_TEST(StringOccurencesOfChar("@3gfd4@432h@", '@') == 3);
    return true;
}

static bool StrDupCheck() {
    ASSERT_TEST(StrDuplicate(NULL) == NULL);
    char* str = StrDuplicate("hello");
    ASSERT_TEST(strcmp("hello", str) == 0);
    free(str);
    return true;
}

static bool GetTimesCheck() {
    int x, y;
    char* str = StrDuplicate("224-6238");
    ASSERT_TEST(GetTimes(str, &x, &y) == TRUE);
    ASSERT_TEST(x == 224);
    ASSERT_TEST(y == 6238);
    free(str);
    return true;
}

static bool CheckLegalHoursCheck() {
    ASSERT_TEST(CheckLegalHours(NULL) == FALSE);
    char* str1 = StrDuplicate("17-23"); 
    char* str2 = StrDuplicate("17-25"); 
    char* str3 = StrDuplicate("25 - 28");
    char* str4 = StrDuplicate("17-16");
    ASSERT_TEST(CheckLegalHours(str1) == TRUE);
    ASSERT_TEST(CheckLegalHours(str2) == FALSE);
    ASSERT_TEST(CheckLegalHours(str3) == FALSE);
    ASSERT_TEST(CheckLegalHours(str4) == FALSE);
    free(str1);
    free(str2);
    free(str3);
    free(str4);
    return true;
}

static bool CheckLegalDayTimeCheck() {
    char* str1 = StrDuplicate("2-23");
    char* str2 = StrDuplicate("3-25");
    ASSERT_TEST(CheckLegalHours(str1) == TRUE);
    ASSERT_TEST(CheckLegalHours(str2) == FALSE);
    free(str1);
    free(str2);
    return true;
}

static bool CreateStringCheck() {
    char* str1 = CreateString(0, 0);
    char* str2 = CreateString(15, 5);
    ASSERT_TEST(strcmp(str1, "0-0") == 0);
    ASSERT_TEST(strcmp(str2, "15-5") == 0);
    free(str1);
    free(str2);
    return true;
}

static bool ConcatListsCheck() {
    List origin = listCreate(CopyString, FreeString);
    List add = listCreate(CopyString, FreeString);
    ASSERT_TEST(ConcatLists(NULL, NULL, CopyString, FreeString) == NULL);
    ASSERT_TEST(ConcatLists(origin, add, NULL, FreeString) == NULL);
    ASSERT_TEST(ConcatLists(origin, add, CopyString, NULL) == NULL);
    ASSERT_TEST(listInsertLast(origin, "what") == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(origin, "is") == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(origin, "better") == LIST_SUCCESS);
    List new_list = ConcatLists(origin, add, CopyString, FreeString);
    ASSERT_TEST(new_list != NULL);
    ASSERT_TEST(ListsEqual(origin, new_list));
    ASSERT_TEST(listInsertLast(add, "to") == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(add, "be") == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(add, "born") == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(add, "good") == LIST_SUCCESS);
    List concat_list = ConcatLists(origin, add, CopyString, FreeString);
    ASSERT_TEST(concat_list != NULL);
    List test_list = listCreate(CopyString, FreeString);
    ASSERT_TEST(listInsertLast(test_list, "what") == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(test_list, "is") == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(test_list, "better") == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(test_list, "to") == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(test_list, "be") == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(test_list, "born") == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(test_list, "good") == LIST_SUCCESS);
    ASSERT_TEST(ListsEqual(concat_list, test_list));
    listDestroy(origin);
    listDestroy(add);
    listDestroy(new_list);
    listDestroy(concat_list);
    listDestroy(test_list);
    return true;
}
int main () {
    RUN_TEST(StringOccCheck);
    RUN_TEST(StrDupCheck);
    RUN_TEST(GetTimesCheck);
    RUN_TEST(CheckLegalHoursCheck);
    RUN_TEST(CheckLegalDayTimeCheck);
    RUN_TEST(CreateStringCheck);
    RUN_TEST(ConcatListsCheck);
    printf("Done! \n");
    return 0;
}