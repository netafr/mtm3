#include "Faculty.h"
#include "EscapyCompany.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>

struct faculty_t {
    TechnionFaculty faculty_name;
    int profit;
    Set companies;
};

/* Function receives a faculty name and creates a new faculty. Returns NULL in 
    case of memory problem. Enter only legal faculty name. */
Faculty FacultyCreate (TechnionFaculty new_name) {
    assert(new_name >= 0 && new_name < UNKNOWN);
    Faculty faculty = malloc(sizeof(*faculty));
    if (faculty == NULL) {
        return NULL;
    }
    faculty -> faculty_name = new_name;
    faculty -> profit = 0;
    faculty -> companies = setCreate(CompanyCopy, CompanyDestroy, 
                                                              CompanyCompare);
    if(faculty -> companies == NULL) {
        free(faculty);
        return NULL;
    }
    return faculty;
}

/* Function receives a faculty to destroy, destroys the set it contains and 
   frees the faculty allocation. */
void FacultyDestroy (Faculty faculty) {
    if(faculty == NULL) {
        return;
    }
    setDestroy(faculty -> companies);
    free(faculty);
}

/* Function receives a faculty and creates a new one with the same data. If
    there is memory problem or faculty is NULL will return NULL. */
Faculty FacultyCopy (Faculty faculty) {
    if(faculty == NULL) {
        return NULL;
    }
    Faculty new_faculty = malloc(sizeof*(new_faculty));
    if(new_faculty == NULL) {
        return NULL;
    }
    new_faculty -> profit = faculty -> profit;
    new_faculty -> faculty_name = faculty -> faculty_name;
    new_faculty -> companies = setCopy(faculty -> companies);
    if(new_faculty -> companies == NULL) {
        free(new_faculty);
        return NULL;
    }
    return new_faculty;
}

/* Function receives 2 faculties and compares them, if the first is greater 
    reutrns > 0. Must send both not NULL. Compares them by the faculty name. */
int FacultyCompare (Faculty first, Faculty second) {
    assert (first && second);
    return ((first -> faculty_name) - (second -> faculty_name));
}

/* Function receives 2 faculties and compares them, if the first is greater 
    reutrns > 0. Must send both not NULL. Compares them by the faculty profit.*/
int FacultyProfitCompare (Faculty first, Faculty second) {
    assert (first && second);
    return ((first -> profit) - (second -> profit));
}

/* Function receives faculty and returns its name (enum). Returns -1 if NULL. */
TechnionFaculty FacultyGetName (Faculty faculty) {
    if (faculty == NULL) {
        return -1;
    }
    return faculty -> faculty_name;
}

/* Function receives faculty and profit and adds the given profit to the faculty
    profit field. Make sure profit is > 0. */
void FacultyAddProfit (Faculty faculty, int profit) {
    if (faculty == NULL) {
        return;
    }
    assert (profit > 0);
    faculty -> profit += profit;
}

/* Function receives faculty and returns its profit. If it's NULL return -1. */
int FacultyGetProfit (Faculty faculty) {
    if (faculty == NULL) {
        return -1;
    }
    return faculty -> profit;
}

/* Function receives faculty and email and returns wether exists a company with
    the given email in the faculty. If one of them is NULL returns false. */
bool FacultyCompanyExists(Faculty faculty, char* email) {
    if(faculty == NULL || email == NULL) {
        return false;
    }
    if(setGetSize(faculty -> companies) == 0) {
        return false;
    }
    return setIsIn(faculty -> companies, FacultyGetCompany(faculty, email));
}

/* Function receives faculty and company, and inserts the company into the 
    faculty's companies set. If one of the parameters are NULL returns FACULTY_
    INVALID_PARAMETER, if there is memory problem returns FACULTY_OUT_OF_MEMORY
    if the company already exists returns FACULTY_COMPANY_EXISTS, otherwise
    returns FACULTY_SUCCESS. */
FacultyErr FacultyInsertCompany(Faculty faculty, EscapeCompany company) {
    if(faculty == NULL || company == NULL) {
        return FACULTY_INVALID_PARAMETER;
    }
    SetResult insert_result = setAdd(faculty -> companies, (void*)company);
    if(insert_result == SET_OUT_OF_MEMORY) {
        return FACULTY_OUT_OF_MEMORY;
    } else if(insert_result == SET_ITEM_ALREADY_EXISTS) {
        return FACULTY_COMPANY_EXISTS;
    }
    return FACULTY_SUCCESS;
}

/* Function receives faculty and email and returns the company with the given
    email within the faculty. If one of the parameters are NULL returns NULL,
    if not found returns NULL.*/
EscapeCompany FacultyGetCompany(Faculty faculty, char* email) {
    if(faculty == NULL || email == NULL) {
        return NULL;
    }
    SET_FOREACH(EscapeCompany, curr_company, faculty -> companies) {
        if(strcmp(CompanyGetEmail(curr_company), email) == 0) {
            return curr_company;
        }
    }
    return NULL;
}

/* Function receives faculty and company and removes the company from the 
    faculty's set. If one of the parameters are NULL we return FACULTY_INVALID_
    PARAMETER, if the company doesn't exist in the faculty we return FACULTY_
    INVALID_PARAMETER, otherwise we return FACULTY_SUCCESS. */
FacultyErr FacultyRemoveCompany(Faculty faculty, EscapeCompany company) {
    if(faculty == NULL || company == NULL) {
        return FACULTY_INVALID_PARAMETER;
    }
    SetResult remove_result = setRemove(faculty -> companies, (void*)company);
    if(remove_result != SET_SUCCESS) {
        return FACULTY_INVALID_PARAMETER;
    } 
    return FACULTY_SUCCESS;
}

/* Function receives faculty, id and pointer to company and returns the room
    in the faculty with the given id, and if company isnt null returns the 
    room's company in it. If faculty is NULL or id is negative we return NULL,
    also when the room isn't found. */
EscapeRoom FacultyGetRoom(Faculty faculty, int id, EscapeCompany* company) {
    if(faculty == NULL || id <= 0) {
        return NULL;
    }
    EscapeRoom room;
    SET_FOREACH(EscapeCompany, curr_company, faculty -> companies) {
        if(CompanyRoomExists(curr_company, id)) {
            room = CompanyGetRoom(curr_company, id);
            if(room == NULL) {
                if(company != NULL) {
                    (*company) = NULL;
                }
                return NULL;
            }
            if(company != NULL) {
                    (*company) = curr_company;
            }
            return room;
        }
    }
    if(company != NULL) {
        (*company) = NULL;
    }
    return NULL;
}

/* Function receives faculty, email, hour and day and checks if there is a user
    with a booking in the given hour and day in one of the facultiy's rooms. If
    one of the parameteres are NULL returns false.*/
bool FacultyUserHasBookings(Faculty faculty, char* email, int hour, int day) {
    if(faculty == NULL || email == NULL) {
        return false;
    }
    SET_FOREACH(EscapeCompany, curr_company, faculty -> companies) {
        if(CompanyUserHasBookings(curr_company, email, hour, day)) {
            return true;
        }
    }
    return false;
}

/* Function receives faculty and returns if it has any rooms at all. If its NULL
    returns false. */
bool FacultyHasRooms(Faculty faculty) {
    if(faculty == NULL) {
        return false;
    }
    int count = 0;
    SET_FOREACH(EscapeCompany, curr_company, faculty -> companies) {
        if(!CompanyHasRooms(curr_company)) {
            count++;
        }
    }
    if(count == setGetSize(faculty -> companies)) {
        return false;
    }
    return true;
}

/* Function receives faculty, level, num_ppl and pointer to int score. It 
    returns the recommended room by the given formula with level and num_ppl, 
    and puts into the score parmeter the score value (of the formula). If one
    of the parameters are NULL or the faculty has no rooms returns NULL. */
EscapeRoom FacultyGetRecommenedRoom(Faculty faculty, int level, int num_ppl, 
                                                                int* score) {
    if(faculty == NULL || score == NULL) {
        return NULL;
    }
    int minScore, minId, tempScore, tempId;
    EscapeRoom minRoom = CompanyGetRecommendedRoom(setGetFirst(faculty -> 
                                companies), level, num_ppl, &minId, &minScore);
    EscapeRoom tempRoom;
    SET_FOREACH(EscapeCompany, curr_company, faculty -> companies) {
        tempRoom = CompanyGetRecommendedRoom(curr_company, level, num_ppl, 
                                                        &tempId, &tempScore);
        if(minScore < 0) {
            minScore = tempScore;
            minId = tempId;
            minRoom = tempRoom;
        }
        else if(tempScore < minScore) {
            minScore = tempScore;
            minId = tempId;
            minRoom = tempRoom;
        } else if(tempScore == minScore && tempId < minId) {
            minScore = tempScore;
            minId = tempId;
            minRoom = tempRoom;
        }
    }
    *(score) = minScore;
    return minRoom;
}

/* Function receives faculty and returns a list of all the faculty's bookings
    that occured (people came) today. If faculty is NULL or there is a memory 
    problem returns NULL. */
List FacultyGetTodayList(Faculty faculty) {
    if(faculty == NULL) {
        return NULL;
    }
    List list = listCreate(BookingCopy, BookingDestroy);
    if(list == NULL) {
        return NULL;
    }
    SET_FOREACH(EscapeCompany, curr_company, faculty -> companies) {
        List temp = CompanyGetTodayList(curr_company);
        if(temp == NULL) {
            return NULL;
        }
        UtilsResult res= ConcatLists(list, temp, BookingCopy, BookingDestroy);
        if(res == ERROR) {
            listDestroy(temp);
            listDestroy(list);
            return NULL;
        }
        listDestroy(temp);
    }
    return list;
}