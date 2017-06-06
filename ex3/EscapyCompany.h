#ifndef COMPANY_H
#define COMPANY_H
#include "set.h"
#include "list.h"
#include "mtm_ex3.h"
#include "EscapeRoom.h"

typedef struct escape_company_t *EscapeCompany;
typedef enum {
    COMPANY_SUCCESS,
    COMPANY_INVALID_PARAMETER,
    COMPANY_OUT_OF_MEMORY,
} CompanyErr;

EscapeCompany CompanyCreate (char* email);
void CompanyDestroy (void* company);
void* CompanyCopy (void* company);
int CompanyCompare (void* company1, void* company2);
char* CompanyGetEmail(EscapeCompany company);
bool CompanyHasBookings(EscapeCompany company);
bool CompanyRoomExists(EscapeCompany company, int id);
CompanyErr CompanyInsertRoom(EscapeCompany company, EscapeRoom room);
CompanyErr CompanyRemoveRoom(EscapeCompany company, EscapeRoom room);
EscapeRoom CompanyGetRoom(EscapeCompany company, int id);
bool CompanyUserHasBookings(EscapeCompany company, char* email, int hour, 
                                                                    int day);
bool CompanyHasRooms(EscapeCompany company);
EscapeRoom CompanyGetRecommendedRoom(EscapeCompany company, int level, int
                                                num_ppl, int* id, int* score);
List CompanyGetTodayList(EscapeCompany company);
#endif