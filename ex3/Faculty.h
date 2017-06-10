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

/* Function receives a faculty name and creates a new faculty. Returns NULL in 
    case of memory problem. Enter only legal faculty name. */
Faculty FacultyCreate (TechnionFaculty name);

/* Function receives a faculty to destroy, destroys the set it contains and 
   frees the faculty allocation. */
void FacultyDestroy (Faculty faculty);

/* Function receives a faculty and creates a new one with the same data. If
    there is memory problem or faculty is NULL will return NULL. */
Faculty FacultyCopy (Faculty faculty);

/* Function receives 2 faculties and compares them, if the first is greater 
    reutrns > 0. Must send both not NULL. Compares them by the faculty name. */
int FacultyCompare (Faculty first, Faculty second);

/* Function receives 2 faculties and compares them, if the first is greater 
    reutrns > 0. Must send both not NULL. Compares them by the faculty profit.*/
int FacultyProfitCompare (Faculty first, Faculty second);

/* Function receives faculty and returns its name (enum). Returns -1 if NULL. */
TechnionFaculty FacultyGetName (Faculty faculty);

/* Function recevies faculty and profit and adds the given profit to the faculty
    profit field. Make sure profit is > 0. */
void FacultyAddProfit (Faculty faculty, int profit);

/* Function receives faculty and returns its profit. If it's NULL return -1. */
int FacultyGetProfit (Faculty faculty);

/* Function receives faculty and email and returns wether exists a company with
    the given email in the faculty. If one of them is NULL returns false. */
bool FacultyCompanyExists(Faculty faculty, char* email);

/* Function receives faculty and company, and inserts the company into the 
    faculty's companies set. If one of the parameters are NULL returns FACULTY_
    INVALID_PARAMETER, if there is memory problem returns FACULTY_OUT_OF_MEMORY
    if the company already exists returns FACULTY_COMPANY_EXISTS, otherwise
    returns FACULTY_SUCCESS. */
FacultyErr FacultyInsertCompany(Faculty faculty, EscapeCompany company);

/* Functionv receives faculty and email and returns the company with the given
    email within the faculty. If one of the parameters are NULL returns NULL,
    if not found returns NULL.*/
EscapeCompany FacultyGetCompany(Faculty faculty, char* email);

/* Function receives faculty and company and removes the company from the 
    faculty's set. If one of the parameters are NULL we return FACULTY_INVALID_
    PARAMETER, if the company doesn't exist in the faculty we return FACULTY_
    INVALID_PARAMETER, otherwise we return FACULTY_SUCCESS. */
FacultyErr FacultyRemoveCompany(Faculty faculty, EscapeCompany company);

/* Function receives faculty, id and pointer to company and returns the room
    in the faculty with the given id, and if company isnt null returns the 
    room's company in it. If faculty is NULL or id is negative we return NULL,
    also when the room isn't found. */
EscapeRoom FacultyGetRoom(Faculty faculty, int id, EscapeCompany* company);

/* Function receives faculty, email, hour and day and checks if there is a user
    with a booking in the given hour and day in one of the facultiy's rooms. If
    one of the parameteres are NULL returns false.*/
bool FacultyUserHasBookings(Faculty faculty, char* email, int hour, int day);

/* Function receives faculty and returns if it has any rooms at all. If its NULL
    returns false. */
bool FacultyHasRooms(Faculty faculty);

/* Function receives faculty, level, num_ppl and pointer to int score. It 
    returns the recommended room by the given formula with level and num_ppl, 
    and puts into the score parmeter the score value (of the formula). If one
    of the parameters are NULL or the faculty has no rooms returns NULL. */
EscapeRoom FacultyGetRecommenedRoom(Faculty faculty, int level, int num_ppl, 
                                                                int* score);

/* Function receives faculty and returns a list of all the faculty's bookings
    that occured (people came) today. If faculty is NULL or there is a memory 
    problem returns NULL. */                                                                
List FacultyGetTodayList(Faculty faculty);

#endif