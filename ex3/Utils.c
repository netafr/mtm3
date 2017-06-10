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
    the 2 parts of the str, seperated by '-'. Send only not NULL parameters. */
UtilsResult GetTimes(char* str, int* first, int* second) {
    assert(str != NULL && first != NULL && second != NULL);
    str = strtok(str, "-");
    *(first) = atoi(str);
    str = strtok(NULL, "-");
    *(second) = atoi(str);
    return TRUE;
}

/* This function gets char* working_hours and returns whether its in the right
    and legal format of working_hours as described. */
UtilsResult CheckLegalHours(char* working_hours) {
    if(working_hours == NULL) {
        return FALSE;
    }
    int opening, closing;
    char* temp = StrDuplicate(working_hours);
    if(temp == NULL) {
        return ERROR;
    }
    UtilsResult res = GetTimes(temp, &opening, &closing);
    if(res == ERROR) {
        free(temp);
        return ERROR;
    }
    if(opening < 0 || opening > 24 || closing < 0 || closing > 24 || 
                                                        opening >= closing) {
        free(temp);
        return FALSE;                                                    
    }
    free(temp);
    return TRUE;
}

/* This function gets char* time and returns wheter its in the right and legal
    format of time as described. */
UtilsResult CheckLegalDayTime(char* time) {
    assert(time != NULL);
    int day, hour;
    char* temp = StrDuplicate(time);
    if(temp == NULL) {
        return ERROR;
    }
    UtilsResult res = GetTimes(temp, &day, &hour);
    if(res == ERROR) {
        free(temp);
        return ERROR;
    }
    if(day < 0 || hour < 0 || hour > 23) {
        free(temp);
        return FALSE;
    }
    free(temp);
    return TRUE;
}

// This function gets a int n and returns its length.
static int GetIntLen(int n) {
    int i = 1;
    while(n > 0) {
        n /= 10;
        i++;
    }
    return i;
}

/* This function receives two integers. It will make a new string, contains both
   integers, with a "-" symbol between. */
char* CreateString(int x, int y) {
    int x_len = GetIntLen(x), y_len = GetIntLen(y);
    char* str = malloc(x_len + y_len + 2); //1 for "/0" and one for "-".
    if(str == NULL) {
        return NULL;
    }
    sprintf(str, "%d-%d", x, y);
    return str;
}

/* This function get 2 lists, origin and add, pointer to CopyListElement 
    function and pointer to FreeListElemnt function. The function concats the
    add list to the origin list. If both are NULL or one of the function pointer
    are NULL or there is a memory problem it returns ERROR, otherwise TRUE. */
UtilsResult ConcatLists(List origin, List add, CopyListElement copy_element, 
                                                FreeListElement list_element) {
    if((origin == NULL && add == NULL) || copy_element == NULL || list_element 
                                                                == NULL) {
        return;
    } else if(origin == NULL) {
        listClear(origin);
        origin = listCopy(add);
    } else if(add == NULL) {
        return;
    }
    if(listGetSize(add) == 0) {
        return ;
    }
    ListResult insert_result = listInsertLast(origin, listGetFirst(add));
    if(insert_result != LIST_SUCCESS) {
        return;
    }
    for(int i = 1; i < listGetSize(add); ++i) {
        ListResult insert_result = listInsertLast(origin, listGetNext(add));
        if(insert_result != LIST_SUCCESS) {
            return;
        }
    }
}