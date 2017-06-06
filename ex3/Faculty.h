#ifndef FACULTY_H
#define FACULTY_H
#include "set.h"
#include "EscapyCompany.h"
#include "list.h"
#include "mtm_ex3.h"

typedef struct faculty_t *Faculty;

typedef enum {
    FACULTY_SUCCESS,
    FACULTY_OUT_OF_MEMORY,
    FACULTY_INVALID_PARAMETER,
    FACULTY_COMPANY_EXISTS,
} FacultyErr;
/* Function */
Faculty FacultyCreate (TechnionFaculty name);
void FacultyDestroy (Faculty faculty);
Faculty FacultyCopy (Faculty faculty);
int FacultyCompare (Faculty first, Faculty second);
int FacultyProfitCompare (Faculty first, Faculty second);
TechnionFaculty FacultyGetName (Faculty faculty);
void FacultySetProfit (Faculty faculty, int profit);
int FacultyGetProfit (Faculty faculty);
bool FacultyCompanyExists(Faculty faculty, char* email);
FacultyErr FacultyInsertCompany(Faculty faculty, EscapeCompany company);
EscapeCompany FacultyGetCompany(Faculty faculty, char* email);
FacultyErr FacultyRemoveCompany(Faculty faculty, EscapeCompany company);
EscapeRoom FacultyGetRoom(Faculty faculty, int id, EscapeCompany company);
bool FacultyUserHasBookings(Faculty faculty, char* email, int hour, int day);
bool FacultyHasRooms(Faculty faculty);
EscapeRoom FacultyGetRecommenedRoom(Faculty faculty, int level, int num_ppl, 
                                                                int* score);
List FacultyGetTodayList(Faculty faculty);
#endif