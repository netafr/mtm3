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
    return setIsIn(faculty -> companies, email);
}

MtmErrorCode FacultyInsertCompany(Faculty faculty, EscapeCompany company) {
    assert(faculty != NULL && company != NULL);
    SetResult insert_result = setAdd(faculty -> companies, (void*)company);
    if(insert_result == SET_OUT_OF_MEMORY) {
        return MTM_OUT_OF_MEMORY;
    }
    return MTM_SUCCESS;
}

EscapeCompany FacultyGetCompany(Faculty faculty, char* email) {
    assert(faculty != NULL && email != NULL);
    EscapeCompany curr_company;
    SET_FOREACH(EscapeCompany, curr_company, faculty -> companies) {
        if(strcmp(CompanyGetEmail(curr_company), email) == 0) {
            return curr_company;
        }
    }
    return NULL;
}

MtmErrorCode FacultyRemoveCompany(Faculty faculty, EscapeCompany company) {
    assert(faculty != NULL && company != NULL);
    SetResult remove_result = setRemove(faculty -> companies, (void*)company);
    if(remove_result != SET_SUCCESS) {
        return MTM_INVALID_PARAMETER;
    }
    return MTM_SUCCESS;
}