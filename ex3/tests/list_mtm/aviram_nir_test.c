#include "test_utilities.h"
#include "../../list_mtm1.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

static ListElement copyInt(ListElement num){
	if(num == NULL) {
		return NULL;
	}
	int* new_num = malloc(sizeof(int));
	if(new_num == NULL) {
		return NULL;
	}
	*(int*)new_num = *(int*)num;
	return new_num;
}

static void freeInt(ListElement num){
	free(num);
}

static int compareInt(ListElement num1, ListElement num2) {
  return *(int*)num2 - *(int*)num1;
}

static bool isDevidedBy(ListElement num1, ListFilterKey num2) {
	return (*(int*)num1)%(*(int*)num2)==0;
}

static bool testListCreate() {
	ASSERT_TEST(listCreate(NULL,NULL) == NULL);
	ASSERT_TEST(listCreate(NULL,NULL) == NULL);
	ASSERT_TEST(listCreate(copyInt,NULL) == NULL);
	ASSERT_TEST(listCreate(NULL,freeInt) == NULL);
	return true;
}



static bool testListDestroy() {
	List list = listCreate(copyInt, freeInt);
	ASSERT_TEST(list != NULL);
	int num1 = 1, num2 = 2;
	ASSERT_TEST(listInsertFirst(list, &num1) == LIST_SUCCESS);
	ASSERT_TEST(listInsertLast(list, &num2) == LIST_SUCCESS);
	listDestroy(NULL);
	listDestroy(list);
	return true;
}


static bool testListCopy() {
	List list = listCreate(copyInt, freeInt);
	ASSERT_TEST(list != NULL);
	for(int i=0; i<5; i++) {
		ASSERT_TEST(listInsertLast(list, &i) == LIST_SUCCESS);
	}
	ASSERT_TEST(listCopy(NULL) == NULL);
	List copy_list = listCopy(list);
	ASSERT_TEST(copy_list != NULL);
	ASSERT_TEST(listGetSize(copy_list) == 5);
	int i=0;
	LIST_FOREACH(int *, num, copy_list) {
		ASSERT_TEST(*num == i);
		i++;
	}
	listDestroy(list);
	listDestroy(copy_list);
	return true;
}

static bool testListFilter() {
	List list = listCreate(copyInt, freeInt);
	ASSERT_TEST(list != NULL);
	for (int i=1; i<=20; i++){
		ASSERT_TEST(listInsertFirst(list, &i) == LIST_SUCCESS);
	}
	int key = 3;
	ASSERT_TEST(listFilter(NULL, isDevidedBy, &key) == NULL);
	ASSERT_TEST(listFilter(list, NULL, &key) == NULL);
	List filtered = listFilter(list, isDevidedBy, &key);
	ASSERT_TEST(filtered != NULL);
	ASSERT_TEST(listGetSize(filtered) == 6);
	int i=18;
	LIST_FOREACH(int *, num, filtered) {
		ASSERT_TEST(*num == i);
		i-=3;
	}
	listDestroy(list);
	listDestroy(filtered);
	return true;
}

static bool testListGetSize() {
	List list = listCreate(copyInt, freeInt);
	ASSERT_TEST(list != NULL);
	ASSERT_TEST(listGetSize(NULL) == -1);
	ASSERT_TEST(listGetSize(list) == 0);
	for (int i=1; i<=20; i++){
		ASSERT_TEST(listInsertFirst(list, &i) == LIST_SUCCESS);
		ASSERT_TEST(listGetSize(list) == i);
	}
	for(int i=0; i<2; i++) {
		int *num = listGetFirst(list);
		ASSERT_TEST(num != NULL);
		ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
	}
	ASSERT_TEST(listGetSize(list) == 18);
	ASSERT_TEST(listClear(list) == LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list) == 0);
	listDestroy(list);
	return true;
}

static bool testListGetFirst() {
	List list = listCreate(copyInt, freeInt);
	ASSERT_TEST(list != NULL);
	for (int i=1; i<=20; i++){
		ASSERT_TEST(listInsertLast(list, &i) == LIST_SUCCESS);
	}
	ASSERT_TEST(listGetFirst(NULL) == NULL);
	int *num = listGetFirst(list);
	ASSERT_TEST(num != NULL);
	ASSERT_TEST(*num == 1);
	for(int i=0; i<2; i++) {
		num = listGetFirst(list);
		ASSERT_TEST(num != NULL);
		ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
	}
	num = listGetFirst(list);
	ASSERT_TEST(num != NULL);
	ASSERT_TEST(*num == 3);
	listDestroy(list);
	return true;
}

static bool testListGetNextAndCurrent() {
	List list = listCreate(copyInt, freeInt);
	ASSERT_TEST(list != NULL);
	ASSERT_TEST(listGetNext(NULL) == NULL);
	ASSERT_TEST(listGetNext(list) == NULL);
	ASSERT_TEST(listGetCurrent(NULL) == NULL);
	ASSERT_TEST(listGetCurrent(list) == NULL);
	for (int i=1; i<=20; i++){
		ASSERT_TEST(listInsertLast(list, &i) == LIST_SUCCESS);
	}
	listGetFirst(list);
	for(int i=2; i<=20; i++) {
		int *num = listGetCurrent(list);
		ASSERT_TEST(num != NULL);
		ASSERT_TEST(*num == i-1);
		num = listGetNext(list);
		ASSERT_TEST(num != NULL);
		ASSERT_TEST(*num == i);
	}
	ASSERT_TEST(listGetNext(list) == NULL);
	ASSERT_TEST(listGetCurrent(list) == NULL);
	listDestroy(list);
	return true;
}

static bool testListInsertFirst() {
	List list = listCreate(copyInt, freeInt);
	ASSERT_TEST(list != NULL);
	int num = 5;
	ASSERT_TEST(listInsertFirst(NULL, &num) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listInsertFirst(list, NULL) == LIST_OUT_OF_MEMORY);
	for (int i=1; i<=20; i++){
		ASSERT_TEST(listInsertFirst(list, &i) == LIST_SUCCESS);
	}
	ASSERT_TEST(listGetSize(list) == 20);
	int i = 20;
	LIST_FOREACH(int*, element, list) {
		ASSERT_TEST(*element == i);
		i--;
	}
	ASSERT_TEST(i == 0);
	listDestroy(list);
	return true;
}

static bool testListInsertLast() {
	List list = listCreate(copyInt, freeInt);
	ASSERT_TEST(list != NULL);
	int num = 5;
	ASSERT_TEST(listInsertLast(NULL, &num) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listInsertLast(list, NULL) == LIST_OUT_OF_MEMORY);
	for (int i=1; i<=20; i++){
		ASSERT_TEST(listInsertLast(list, &i) == LIST_SUCCESS);
	}
	ASSERT_TEST(listGetSize(list) == 20);
	int i = 1;
	LIST_FOREACH(int*, element, list) {
		ASSERT_TEST(*element == i);
		i++;
	}
	ASSERT_TEST(i == 21);
	listDestroy(list);
	return true;
}

static bool testListInsertBeforeAndAfterCurrent() {
	List list = listCreate(copyInt, freeInt);
	ASSERT_TEST(list != NULL);
	for (int i=2; i<=10; i+=2){
		ASSERT_TEST(listInsertLast(list, &i) == LIST_SUCCESS);
	}
	int i = 1;
	ASSERT_TEST(listInsertBeforeCurrent(NULL, &i) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listInsertBeforeCurrent(list, &i) == LIST_INVALID_CURRENT);
	listGetFirst(list);
	ASSERT_TEST(listInsertBeforeCurrent(list, NULL) == LIST_OUT_OF_MEMORY);

	LIST_FOREACH(int*, element, list) {
		ASSERT_TEST(listInsertBeforeCurrent(list, &i) == LIST_SUCCESS);
		i+=2;
	}
	ASSERT_TEST(listGetSize(list) == 10);
	i = 1;
	LIST_FOREACH(int*, element, list) {
		ASSERT_TEST(*element == i);
		i++;
	}

	ASSERT_TEST(listClear(list) == LIST_SUCCESS);
	for (i=1; i<=10; i+=2){
		ASSERT_TEST(listInsertLast(list, &i) == LIST_SUCCESS);
	}
	i = 2;
	ASSERT_TEST(listInsertAfterCurrent(NULL, &i) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listInsertAfterCurrent(list, &i) == LIST_INVALID_CURRENT);
	listGetFirst(list);
	ASSERT_TEST(listInsertAfterCurrent(list, NULL) == LIST_OUT_OF_MEMORY);
	LIST_FOREACH(int*, element, list) {
		ASSERT_TEST(listInsertAfterCurrent(list, &i) == LIST_SUCCESS);
		i+=2;
		element = listGetNext(list); //skipping what we added
	}
	ASSERT_TEST(listGetSize(list) == 10);
	i = 1;
	LIST_FOREACH(int*, element, list) {
		ASSERT_TEST(*element == i);
		i++;
	}
	listDestroy(list);
	return true;
}

static bool testListRemoveCurrent() {
	List list = listCreate(copyInt, freeInt);
	ASSERT_TEST(list != NULL);
	ASSERT_TEST(listRemoveCurrent(NULL) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);
	for (int i=1; i<=10; i++){
		ASSERT_TEST(listInsertLast(list, &i) == LIST_SUCCESS);
	}
	ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);
	// removing all even numbers
	LIST_FOREACH(int*, element, list) {
		if((*element)%2 == 0) {
			ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
			element = listGetFirst(list);
		}
	}
	ASSERT_TEST(listGetSize(list) == 5);
	int i=1;
	LIST_FOREACH(int*, element, list) {
		ASSERT_TEST(*element == i);
		i+=2;
	}
	listDestroy(list);
	return true;
}

static bool testListSort() {
	List list = listCreate(copyInt, freeInt);
	ASSERT_TEST(list != NULL);
	ASSERT_TEST(listSort(NULL, compareInt) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listSort(list, NULL) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listSort(list, compareInt) == LIST_SUCCESS);
	for (int i=1; i<=10; i++){
		ASSERT_TEST(listInsertFirst(list, &i) == LIST_SUCCESS);
		if(i == 7) {
			ASSERT_TEST(listInsertFirst(list, &i) == LIST_SUCCESS);
		}
	}
	int *current = listGetFirst(list);
	current = listGetNext(list);
	ASSERT_TEST(current != NULL);
	ASSERT_TEST(*current = 9);
	ASSERT_TEST(listSort(list, compareInt) == LIST_SUCCESS);
	current = listGetCurrent(list);
	ASSERT_TEST(current != NULL);
	ASSERT_TEST(*current = 2);
	int i=1;
	LIST_FOREACH(int*, element, list) {
		ASSERT_TEST(*element == i);
		if(i == 7) {
			element = listGetNext(list);
			ASSERT_TEST(*element == i);
		}
		i++;
	}
	listDestroy(list);
	return true;
}

static bool testListClear() {
	List list = listCreate(copyInt, freeInt);
	ASSERT_TEST(list != NULL);
	ASSERT_TEST(listClear(NULL) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listClear(list) == LIST_SUCCESS);
	for (int i=1; i<=10; i++){
		ASSERT_TEST(listInsertLast(list, &i) == LIST_SUCCESS);
	}
	listGetFirst(list);
	listGetNext(list); //iterator is not NULL
	ASSERT_TEST(listClear(list) == LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list) == 0);
	ASSERT_TEST(listGetCurrent(list) == NULL);
	int num = 5;
	ASSERT_TEST(listInsertAfterCurrent(list, &num) == LIST_INVALID_CURRENT);
	listDestroy(list);
	return true;
}



int main (int argv, char** arc){
	setvbuf(stdout, NULL, _IONBF, 0);
	RUN_TEST(testListCreate);
	RUN_TEST(testListDestroy);
	RUN_TEST(testListCopy);
	RUN_TEST(testListFilter);
	RUN_TEST(testListGetSize);
	RUN_TEST(testListGetFirst);
	RUN_TEST(testListGetNextAndCurrent);
	RUN_TEST(testListInsertFirst);
	RUN_TEST(testListInsertLast);
	RUN_TEST(testListInsertBeforeAndAfterCurrent);
	RUN_TEST(testListRemoveCurrent);
	RUN_TEST(testListSort);
	RUN_TEST(testListClear);
	return 0;
}


