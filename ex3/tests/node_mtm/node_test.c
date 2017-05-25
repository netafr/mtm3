#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

#include "test_utilities.h"
#include "list.h"

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

static bool testNodeCreate() {
	ASSERT_TEST(NodeCreate(NULL,copyString, freeString()) == NULL);
	return true;
}
static bool testGetNext(Node node) {
    ASSERT_TEST(GetNext(node) == node -> next);
    return true;
}

static bool testNodeInsert(Node node, Node target)