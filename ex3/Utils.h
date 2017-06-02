#ifndef UTILS_H
#define UTILS_H
#include <malloc.h>
#include <string.h>

int StringOccurencesOfChar(char* string, char look_for);
char* StrDuplicate(char* str);
void GetTimes(char* str, int* first, int* second);
bool CheckLegalHours(char* working_hours);
bool CheckLegalDayTime(char* time);
#endif