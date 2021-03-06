#ifndef UTILS_H
#define UTILS_H
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "mtm_ex3.h"
#include "list.h"

typedef enum {
    TRUE,
    FALSE,
    ERROR,
} UtilsResult;

/* This function gets char* str and char look_for and returns the number of
    occurences of look_for in str. */
int StringOccurencesOfChar(char* string, char look_for);

/* This funcion gets char* str, and returns a new char* which is exacpt copy of
    the given str. It mallocs memory for the new string. Returns NULL if str
    is NULL or there was a memory problem. */
char* StrDuplicate(char* str);

/* This function gets char* str and 2 pointers to ints, and puts in each of them
    the 2 parts of the str, seperated by '-'. Send only not NULL parameters. */
UtilsResult GetTimes(char* str, int* first, int* second);

/* This function gets char* working_hours and returns whether its in the right
    and legal format of working_hours as described. */
UtilsResult CheckLegalHours(char* working_hours);

/* This function gets char* time and returns wheter its in the right and legal
    format of time as described. */
UtilsResult CheckLegalDayTime(char* time);

/* This function receives two integers. It will make a new string, contains both
   integers, with a "-" symbol between. */
char* CreateString(int x, int y);

/* This function get 2 lists, origin and add, pointer to CopyListElement 
    function and pointer to FreeListElemnt function. The function concats the
    add list to the origin list. If both are NULL or one of the function pointer
    are NULL or there is a memory problem it returns ERROR, otherwise TRUE. */
UtilsResult ConcatLists(List origin, List add, CopyListElement copy_element, 
                                                FreeListElement list_element);
#endif