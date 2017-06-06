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

/* Function receives a faculty name and creates a new faculty. */
Faculty FacultyCreate (TechnionFaculty new_name) {
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

int FacultyCompare (Faculty first, Faculty second) {
    assert (first && second);
    return ((first -> faculty_name) - (second -> faculty_name));
}

int FacultyProfitCompare (Faculty first, Faculty second) {
    assert (first && second);
    return ((first -> profit) - (second -> profit));
}

TechnionFaculty FacultyGetName (Faculty faculty) {
    if (faculty == NULL) {
        return 0;
    }
    return faculty -> faculty_name;
}

void FacultySetProfit (Faculty faculty, int profit) {
    if (faculty == NULL) {
        return;
    }
    assert (profit > 0);
    faculty -> profit = profit;
    return;
}

int FacultyGetProfit (Faculty faculty) {
    if (faculty == NULL) {
        return -1;
    }
    return faculty -> profit;
}

bool FacultyCompanyExists(Faculty faculty, char* email) {
    if(faculty == NULL || email == NULL) {
        return false;
    }
    return setIsIn(faculty -> companies, email);
}

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

EscapeRoom FacultyGetRoom(Faculty faculty, int id, EscapeCompany company) {
    if(faculty == NULL || id <= 0) {
        return NULL;
    }
    EscapeRoom room;
    SET_FOREACH(EscapeCompany, curr_company, faculty -> companies) {
        if(CompanyRoomExists(curr_company, id)) {
            room = CompanyGetRoom(curr_company, id);
            if(room == NULL) {
                if(company != NULL) {
                    company = NULL;
                }
                return NULL;
            }
            if(company != NULL) {
                    company = curr_company;
            }
            return room;
        }
    }
    if(company != NULL) {
        company = NULL;
    }
    return NULL;
}

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

EscapeRoom FacultyGetRecommenedRoom(Faculty faculty, int level, int num_ppl, 
                                                                int* score) {
    if(faculty == NULL) {
        return NULL;
    }
    int minScore, minId, tempScore, tempId;
    EscapeRoom minRoom = CompanyGetRecommendedRoom(setGetFirst(faculty -> 
                                companies), level, num_ppl, &minId, &minScore);
    EscapeRoom tempRoom;
    SET_FOREACH(EscapeCompany, curr_company, faculty -> companies) {
        tempRoom = CompanyGetRecommendedRoom(curr_company, level, num_ppl, 
                                                        &tempId, &tempScore);
        if(tempScore < minScore) {
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

List FacultyGetTodayList(Faculty faculty) {
    if(faculty == NULL) {
        return NULL;
    }
    List list = listCreate(BookingCopy, BookingDestroy);
    if(list == NULL) {
        return NULL;
    }
    List temp;
    SET_FOREACH(EscapeCompany, curr_company, faculty -> companies) {
        temp = CompanyGetTodayList(curr_company);
        if(temp == NULL) {
            return NULL;
        }
        list = ConcatLists(list, temp);
    }
    return list;
}