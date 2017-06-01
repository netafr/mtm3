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

/* This function gets char* working_hours and returns whether its in the right
    and legal format of working_hours as described. */
bool CheckLegalHours(char* working_hours) {
    if(working_hours == NULL) {
        return false;
    }
    char* str = strtok(working_hours, "-");
    int opening = atol(str);
    str = strtok(NULL, "-");
    int closing = atol(str);
    if(opening < 0 || opening > 24 || closing < 0 || closing > 24 || 
                                                        opening >= closing) {
        return false;                                                    
    }
    return true;
}