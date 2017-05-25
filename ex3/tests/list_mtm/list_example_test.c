#include "test_utilities.h"
#include "../../list_mtm1.h"
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

static bool isLongerThan(ListElement element,ListFilterKey number) {
	char* string = element;
    return strlen(string) > *(int*)number;
}

static int stringCmp(ListElement first, ListElement second) {
	return strcmp((char*)first, (char*)second);
}

void printList(List list) {
   char* str = listGetFirst(list);
   while(str != NULL) {
		printf("\n");
   		printf("%s ", str);
   		str = listGetNext(list);
   }
   printf("\n");
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
	for (int i = 0; i < 5; ++i){
		listInsertFirst(list,a[i]);
	}
	int key = 5;
	List filtered = listFilter(list,isLongerThan, &key);
	ASSERT_TEST(listGetSize(filtered) == 1);
	ASSERT_TEST(strcmp(listGetFirst(filtered),a[3])==0);
	ASSERT_TEST(listFilter(filtered, NULL, &key) == NULL);
	listDestroy(list);
	listDestroy(filtered);
	return true;
}
static bool testListCopy() {
	ASSERT_TEST(listCopy(NULL) == NULL);
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	for (int i = 0; i < 5; ++i){
		listInsertFirst(list,a[i]);
	}
	char *temp = listGetFirst(list);
	temp = listGetNext(list);
	ASSERT_TEST(temp != NULL);
	List copied = listCopy(list);
	char* first = listGetCurrent(list);
	char* second = listGetCurrent(copied);
	ASSERT_TEST(stringCmp(first, second) == 0);
	first = listGetFirst(list);
	second = listGetFirst(copied);
	while(first != NULL || second != NULL) {
		ASSERT_TEST(stringCmp(first, second) == 0);
		first = listGetNext(list);
		second = listGetNext(copied);
	}
	listDestroy(list);
	listDestroy(copied);
	return true;
}

static bool testListGetSize() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	ASSERT_TEST(listGetSize(list) == 5);
	List list_empty = listCreate(copyString, freeString);
	ASSERT_TEST(listGetSize(list_empty) == 0);
	listDestroy(list);
	listDestroy(list_empty);
	return true;
}

static bool testListGetFirst() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString, freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	ASSERT_TEST(strcmp("I", listGetFirst(list)) == 0);
	List list_empty = listCreate(copyString, freeString);
	ASSERT_TEST(listGetFirst(list_empty) == NULL);
	listDestroy(list_empty);
	listDestroy(list);
	return true;
}

static bool testListGetNext() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString, freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	char* str = listGetFirst(list);
	for(int i = 0; i < 3; ++i) {
		str = listGetNext(list);
	}
	ASSERT_TEST(strcmp("bbb", str) == 0);
	str = listGetNext(list);
	ASSERT_TEST(strcmp("aaa", str) == 0);
	str = listGetNext(list);
	ASSERT_TEST(str == NULL);
	listDestroy(list);
	return true;
}


static bool testListInsertFirst() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString, freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	ListResult insert_result = listInsertFirst(list, "hello");
	ASSERT_TEST(insert_result == LIST_SUCCESS);
	char* str = listGetFirst(list);
	ASSERT_TEST(strcmp("hello", str) == 0);
	ASSERT_TEST(listInsertFirst(NULL, NULL) == LIST_NULL_ARGUMENT);
	listDestroy(list);
	return true;
}

static bool testListInsertLast() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString, freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	ListResult insert_result = listInsertLast(list, "last");
	ASSERT_TEST(insert_result == LIST_SUCCESS);
	char* str = listGetFirst(list);
	for(int i = 0; i < listGetSize(list) - 1; ++i) {
		str = listGetNext(list);
	}
	ASSERT_TEST(strcmp(str, "last") == 0);
	ASSERT_TEST(listInsertLast(NULL, "hi") == LIST_NULL_ARGUMENT);
	listDestroy(list);
	return true;
}

//Until here all tests check all cases (not memory failure ofc).
static bool testListInsertBeforeCurrent() {
	return true;
}

static bool testListInsertAfterCurrent() {
	return true;
}

static bool testListRemoveCurrent() {
	return true;
}

static bool testListSort() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString, freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	printList(list);
	listSort(list, stringCmp);
	printList(list);
	listDestroy(list);
	return true;
}

static bool testListClear() {
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
	RUN_TEST(testListClear);
	RUN_TEST(testListDestroy);
	return 0;
}

