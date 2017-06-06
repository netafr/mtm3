#ifndef UTILS_H
#define UTILS_H
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

typedef enum {
    TRUE,
    FALSE,
    ERROR,
} UtilsResult;

int StringOccurencesOfChar(char* string, char look_for);
char* StrDuplicate(char* str);
UtilsResult GetTimes(char* str, int* first, int* second);
UtilsResult CheckLegalHours(char* working_hours);
UtilsResult CheckLegalDayTime(char* time);
char* CreateString(int x, int y);
#endif