#include "test_utilities.h"
#include "../../list_mtm1.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

static int my_strcmp(ListElement arg1, ListElement arg2){
	return strcmp((char*)arg2, (char*)arg1);
}
static char* testGetLast(List list){
	char *temp=listGetFirst(list);
	char* one_after_temp=listGetNext(list);
	while(one_after_temp){
		temp=one_after_temp;
		one_after_temp=listGetNext(list);
	}
	listGetFirst(list);
	return temp;
}

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

static bool testListCreate() {
	ASSERT_TEST(listCreate(NULL,NULL) == NULL);
	ASSERT_TEST(listCreate(copyString,NULL) == NULL);
	ASSERT_TEST(listCreate(NULL,freeString) == NULL);
	List list=listCreate(copyString, freeString);
	ASSERT_TEST(list != NULL);
	listDestroy(list);
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
	ASSERT_TEST(listCopy(NULL) == NULL);
	char* a[5] = {"aa","bb","","hello mister fish","shalom"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	List list_copy=listCopy(list);
	ASSERT_TEST(list_copy!=NULL);
	ASSERT_TEST(listGetSize(list)==listGetSize(list_copy));
	char* list_iterator=listGetFirst(list);
	char* copy_iterator=listGetFirst(list_copy);
	while(list_iterator&&copy_iterator){
		ASSERT_TEST(strcmp(list_iterator, copy_iterator)==0);
		list_iterator=listGetNext(list);
		copy_iterator=listGetNext(list_copy);
	}
	listDestroy(list);
	listDestroy(list_copy);
	List empty_list = listCreate(copyString,freeString);
	List empty_list_copy=listCopy(empty_list);
	ASSERT_TEST(empty_list_copy!=NULL);
	ASSERT_TEST(listGetSize(empty_list)==listGetSize(empty_list_copy));
	listDestroy(empty_list);
	listDestroy(empty_list_copy);
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
	List empty_list = listCreate(copyString,freeString);
	ASSERT_TEST(listGetSize(empty_list) == 0);
	listDestroy(empty_list);
	return true;
}

static bool testListGetFirst() {
	char* a[6] = {"	","","ABcds","yA","I_LOVE","LIFE"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i <6; ++i){
		listInsertFirst(list,a[i]);
	}
	for (int j = 5; j >=0 ; j--) {
		ASSERT_TEST(0==strcmp(listGetFirst(list),a[j]));
		listRemoveCurrent(list);
	}
	listDestroy(list);
	return true;
}

static bool testListGetNext() {
	ASSERT_TEST(listGetNext(NULL)==NULL);
	List list = listCreate(copyString,freeString);
	listInsertFirst(list, "a");
	ASSERT_TEST(listGetNext(list)==NULL);//invalid iterator
	listGetFirst(list);
	ASSERT_TEST(listGetNext(list) == NULL);//no next
	listInsertLast(list, "b");
	listGetFirst(list);
	ASSERT_TEST(strcmp(listGetNext(list), "b")==0);
	listDestroy(list);
	return true;
}

static bool testListInsertFirst() {
	ASSERT_TEST(listInsertFirst(NULL, "a")==LIST_NULL_ARGUMENT);
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString, freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	ASSERT_TEST(listInsertFirst(list, "first")==LIST_SUCCESS);
	ASSERT_TEST(strcmp(listGetFirst(list), "first")==0);
	listDestroy(list);
	List empty_list = listCreate(copyString,freeString);
	ASSERT_TEST(listInsertFirst(empty_list, "shalom")==LIST_SUCCESS);
	ASSERT_TEST(strcmp(listGetFirst(empty_list), "shalom")==0);
	listDestroy(empty_list);
	return true;
}

static bool testListInsertLast() {
	char* a[6] = {"N","Na","moshe","LION KING!","FOod", "bbb"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i <6; ++i){
		listInsertLast(list,a[i]);
		ASSERT_TEST(0==strcmp(testGetLast(list),a[i]));
	}
	listDestroy(list);
	return true;
}

static bool testListInsertBeforeCurrent() {
	ASSERT_TEST(listInsertBeforeCurrent(NULL, "a")==LIST_NULL_ARGUMENT);
	char* a[2] = {"NY", "city"};
	List list = listCreate(copyString, freeString);
	for (int i=0;i <2; ++i){
		listInsertFirst(list,a[i]);
	}
	ASSERT_TEST(listInsertBeforeCurrent(list, "a")==LIST_INVALID_CURRENT);
	listGetFirst(list);
	ASSERT_TEST(listInsertBeforeCurrent(list, "a")==LIST_SUCCESS);
	ASSERT_TEST(strcmp(listGetFirst(list), "a")==0);
	listGetNext(list);
	ASSERT_TEST(listInsertBeforeCurrent(list, "b")==LIST_SUCCESS);
	listGetFirst(list);
	ASSERT_TEST(strcmp(listGetNext(list), "b")==0);
	for(int i=0; i<3; i++){
		listGetNext(list);
	}
	//ASSERT_TEST(listInsertBeforeCurrent(list, "no")==LIST_INVALID_CURRENT);
	listDestroy(list);
	return true;
}

static bool testListInsertAfterCurrent() {
	char* a[7] = {"OMG","All","tHe","House","is", "GOld", "LINEEEE!!"};
	List list = listCreate(copyString,freeString);
	listInsertFirst(list,"ABC");
	listGetFirst(list);
	for (int i=0;i <7; ++i){
		listInsertAfterCurrent(list,a[i]);
		listGetNext(list);
		ASSERT_TEST(0==strcmp(listGetCurrent(list),a[i]));
	}
	listDestroy(list);
	return true;
}

static bool testListRemoveCurrent() {
	ASSERT_TEST(listRemoveCurrent(NULL)==LIST_NULL_ARGUMENT);
	char* a[5] = {"aaa","bbb","shalom","hello mister fish",""};
	List list = listCreate(copyString, freeString);
	for (int i=0;i <5; ++i){
		listInsertLast(list,a[i]);
	}
	ASSERT_TEST(listRemoveCurrent(list)==LIST_INVALID_CURRENT);
	listGetFirst(list);
	ASSERT_TEST(listRemoveCurrent(list)==LIST_SUCCESS);
	ASSERT_TEST(strcmp(listGetFirst(list), "bbb")==0);
	listGetNext(list);
	ASSERT_TEST(listRemoveCurrent(list)==LIST_SUCCESS);
	ASSERT_TEST(listRemoveCurrent(list)==LIST_INVALID_CURRENT);
	listGetFirst(list);
	ASSERT_TEST(strcmp(listGetNext(list), "hello mister fish")==0);
	listDestroy(list);
	return true;
}

static bool testListSort() {
	char* a[6] = {"a","aa","yarden", "aviv", "matam", "ab"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i <6; ++i){
		listInsertFirst(list,a[i]);
	}
	listSort(list,my_strcmp);
	listGetFirst(list);
	ASSERT_TEST(0==strcmp(listGetCurrent(list),"a"));
	ASSERT_TEST(0==strcmp(listGetNext(list),"aa"));
	ASSERT_TEST(0==strcmp(listGetNext(list),"ab"));
	ASSERT_TEST(0==strcmp(listGetNext(list),"aviv"));
	ASSERT_TEST(0==strcmp(listGetNext(list),"matam"));
	ASSERT_TEST(0==strcmp(listGetNext(list),"yarden"));
	listDestroy(list);
	List second_list = listCreate(copyString,freeString);
	for (int i=0;i <6; ++i){
		listInsertFirst(second_list,a[i]);
	}
	ASSERT_TEST(listSort(second_list,NULL)==LIST_NULL_ARGUMENT);
	ASSERT_TEST(listSort(NULL,my_strcmp)==LIST_NULL_ARGUMENT);
	listDestroy(second_list);
	char* b[5] = {"aaa","NI","hello mister fish","I", "bbb"};
	List list1 = listCreate(copyString,freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list1,b[i]);
	}
	listSort(list1,my_strcmp);
	ASSERT_TEST(strcmp(listGetFirst(list1),"I") == 0);
	ASSERT_TEST(strcmp(listGetNext(list1),"NI") == 0);
	ASSERT_TEST(strcmp(listGetNext(list1),"aaa") == 0);
	ASSERT_TEST(strcmp(listGetNext(list1),"bbb") == 0);
	ASSERT_TEST(strcmp(listGetNext(list1),"hello mister fish") == 0);
	listDestroy(list1);
	return true;
}

static bool testListClear() {
	ASSERT_TEST(listClear(NULL)==LIST_NULL_ARGUMENT);
	char* a[5] = {"aaa","bbb","shalom","hello mister fish",""};
	List list = listCreate(copyString, freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	ASSERT_TEST(listClear(list)==LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list)==0);
	listDestroy(list);
	List empty_list=listCreate(copyString, freeString);
	ASSERT_TEST(listClear(empty_list)==LIST_SUCCESS);
	ASSERT_TEST(listGetSize(empty_list)==0);
	listDestroy(empty_list);
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

