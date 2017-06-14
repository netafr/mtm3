#include "test_utilities.h"
#include "../list_mtm1.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

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

static bool hasLetter(ListElement element,ListFilterKey letter) {
	char* str = element;
	char key = *(char*)letter;
    if (strchr(str, (int)key) != NULL) {
    	return true;
    }
    return false;
}

static int stringCmp(ListElement first, ListElement second) {
	return strcmp((char*)first, (char*)second);
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
	listDestroy(filtered);
	char letterh = 'h';
	char letterq = 'q';
	List filtered2 = listFilter(list, hasLetter, &letterh);
	ASSERT_TEST(listGetSize(filtered2) == 1);
	ASSERT_TEST(strcmp(listGetFirst(filtered2),a[3])==0);
	listDestroy(filtered2);
	List filtered3 = listFilter(list, hasLetter, &letterq);
	ASSERT_TEST(listGetSize(filtered3) == 0);
	listDestroy(filtered3); 
	listDestroy(list);
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

static bool testListInsertBeforeCurrent() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString, freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	ListResult insert_result = listInsertBeforeCurrent(list, "error");
	ASSERT_TEST(insert_result == LIST_INVALID_CURRENT);
	char* str = listGetFirst(list);
	insert_result = listInsertBeforeCurrent(list, "first");
	ASSERT_TEST(insert_result == LIST_SUCCESS);
	str = listGetFirst(list);
	ASSERT_TEST(strcmp(str, "first") == 0);
	str = listGetNext(list);
	ASSERT_TEST(strcmp(str, "I") == 0);
	insert_result = listInsertBeforeCurrent(list, "second");
	ASSERT_TEST(insert_result == LIST_SUCCESS);
	ASSERT_TEST(listInsertBeforeCurrent(NULL, "hi") == LIST_NULL_ARGUMENT);
	listDestroy(list);
	return true;
}

static bool testListInsertAfterCurrent() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString, freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	ListResult insert_result = listInsertAfterCurrent(list, "error");
	ASSERT_TEST(insert_result == LIST_INVALID_CURRENT);
	char* str = listGetFirst(list);
	insert_result = listInsertAfterCurrent(list, "second");
	ASSERT_TEST(insert_result == LIST_SUCCESS);
	ASSERT_TEST(strcmp(str, "I") == 0);
	str = listGetNext(list);
	ASSERT_TEST(strcmp(str, "second") == 0);
	insert_result = listInsertAfterCurrent(list, "third");
	str = listGetNext(list);
	ASSERT_TEST(strcmp(str, "third") == 0);
	ASSERT_TEST(insert_result == LIST_SUCCESS);
	ASSERT_TEST(listInsertAfterCurrent(NULL, "hi") == LIST_NULL_ARGUMENT);
	listDestroy(list);
	return true;
}



static bool testListRemoveCurrent() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString, freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	ListResult remove_result = listRemoveCurrent(list);
	ASSERT_TEST(remove_result == LIST_INVALID_CURRENT);
	char* str = listGetFirst(list);
	remove_result = listRemoveCurrent(list);
	ASSERT_TEST(remove_result == LIST_SUCCESS);
	str = listGetFirst(list);
	ASSERT_TEST(strcmp(str, "hello mister fish") == 0);
	str = listGetNext(list);
	remove_result = listRemoveCurrent(list);
	ASSERT_TEST(remove_result == LIST_SUCCESS);
	ASSERT_TEST(listInsertAfterCurrent(NULL, "hi") == LIST_NULL_ARGUMENT);
	listDestroy(list);
	return true;
}


static bool testListSort() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString, freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	ASSERT_TEST(listSort(list, NULL) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listSort(NULL, stringCmp) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listSort(NULL, NULL) == LIST_NULL_ARGUMENT);
	char* str = listGetFirst(list);
	str = listGetNext(list);
	ASSERT_TEST(strcmp(str, "hello mister fish") == 0);
	ListResult sort_result = listSort(list, stringCmp);
	ASSERT_TEST(sort_result == LIST_SUCCESS);
	str = listGetCurrent(list);
	ASSERT_TEST(strcmp(str, "bbb") == 0);	
	listDestroy(list);
	return true;
}

static bool testListClear() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString, freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	ASSERT_TEST(listClear(list) == LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list) == 0);
	listDestroy(list);
	return true;
}

static bool testListDestroy() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString, freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	listDestroy(list);
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
	printf("Done!\n");
	return 0;
}

