#include "Utils.h"

/* This function gets char* str and char look_for and returns the number of
    occurences of look_for in str. */
int StringOccurencesOfChar(char* str, char look_for) {
    int count = 0;
    for(int i = 0; str[i]; ++i) {
        if(str[i] == look_for) {
            count++;
        }
    }
    return count;
}

/* This funcion gets char* str, and returns a new char* which is exacpt copy of
    the given str. It mallocs memory for the new string. Returns NULL if str
    is NULL or there was a memory problem. */
char* StrDuplicate(char* str) {
    if(str == NULL) {
        return NULL;
    }
    char* new_str = malloc(strlen(str) + 1);
    if(new_str == NULL) {
        return NULL;
    }
    new_str = strcpy(new_str, str);
    return new_str;
}

/* This function gets char* str and 2 pointers to ints, and puts in each of them
    the 2 parts of the str, seperated by '-'. */
void GetTimes(char* str, int* first, int* second) {
    assert(str != NULL && first != NULL && second != NULL);
    char* split = strtok(str, "-");
    *(first) = atol(split);
    split = strtok(NULL, "-");
    *(second) = atol(split);
}

/* This function gets char* working_hours and returns whether its in the right
    and legal format of working_hours as described. */
bool CheckLegalHours(char* working_hours) {
    if(working_hours == NULL) {
        return false;
    }
    int opening, closing;
    GetTimes(working_hours, &opening, &closing);
    if(opening < 0 || opening > 24 || closing < 0 || closing > 24 || 
                                                        opening >= closing) {
        return false;                                                    
    }
    return true;
}

/* This function gets char* time and returns wheter its in the right and legal
    format of time as described. */
bool CheckLegalDayTime(char* time) {
    assert(time != NULL);
    int day, hour;
    GetTimes(time, &day, &hour);
    if(day < 0 || hour < 0 || hour > 23) {
        return false;
    }
    return true;
}