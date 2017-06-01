#include "EscapeTechnion.h"
#include "mtm_ex3.h"
#include "EscapyCompany.h"
#include "set.h"
#include "User.h"
#include "Faculty.h"
#include "Utils.h"
#include <malloc.h>
#include <string.h>

struct escape_technion_t {
    int num_of_faculties;
    Faculty *faculties; //Array of all the faculties
    Set users;
    int day;
};

/* Helper function to free all the faculties created until now if one fails.
    Its inputs are escape_system and int num_to_clear which indicates how many
    faculties are needed to be destroyed. */
static void ClearFaculties(EscapeTechnion escape_system, int num_to_clear) {
    assert(escape_system != NULL);
    assert(escape_system != NULL);
    for(int i = 0; i < num_to_clear; ++i) {
        FacultyDestroy(escape_system -> faculties[i]);
    }
}

/* Helper function that gets escape_system and char* email and returns whether
    or not exists a company with the given email. */
static bool CompanyMailExists(EscapeTechnion escape_system, char* email) {
    for(int i = 0; i < escape_system -> num_of_faculties; ++i) {
        if(FacultyCompanyExists(escape_system -> faculties[i], email)) {
            return true;
        }
    }
    return false;
}

/* Helper function that gets escape_system and char* email and returns wether
    or not the mail already exists anywhere in the system. */
static bool MailExists(EscapeTechnion escape_system, char* email) {
    assert(escape_system != NULL && email != NULL);
    if(setIsIn(escape_system -> users, email)) {
        return true;
    }
    return CompanyMailExists(escape_system, email);
}

/* Helper function that gets escapse_system, char* email and a faculty. It finds
    the company with the given email and returns it, and also puts into faculty
    variable the faculty of that company. faculty is optional parameter. */
static EscapeCompany GetCompany(EscapeTechnion escape_system, char* email, 
                                                            Faculty faculty) {
    assert(escape_system != NULL && email != NULL);
    for(int i = 0; i < escape_system -> num_of_faculties; ++i) {
        if(FacultyCompanyExists(escape_system -> faculties[i], email)) {
            if(faculty != NULL) {
                faculty = escape_system -> faculties[i];
            }
            return FacultyGetCompany(escape_system -> faculties[i], email);
        }
    }
    return NULL;
}

/* Helper function that gets escape_system, int id and char* email and returns
    wether exists a room with the given ID. */
static bool RoomIdExists(EscapeTechnion escape_system, int id, char* email) {
    assert(escape_system != NULL && id > 0 && email != NULL);
    EscapeCompany wanted_company = GetCompany(escape_system, email, NULL);
    return CompanyRoomExists(wanted_company, id);
}

/* This function creates the system upon initializtion, when the program is 
    called. Its mallocs and creates the faculties array and the users set. 
    It initliazes all the faculties. If there is a memory problem it returns
    NULL, otherwise returns the newly created system. */
EscapeTechnion SystemCreate() {
    EscapeTechnion escape_system = malloc(sizeof(*escape_system));
    if(escape_system == NULL) {
        return NULL;
    }
    escape_system -> day = 0;
    escape_system -> num_of_faculties = UNKNOWN;
    escape_system -> faculties = malloc(sizeof(Faculty)*
                                        (escape_system -> num_of_faculties));
    if(escape_system -> faculties == NULL) {
        free(escape_system);
        return NULL;
    }
    escape_system -> users = setCreate(UserCopy, UserDestroy, UserCompare);
    if(escape_system -> users == NULL) {
        free(escape_system -> faculties);
        free(escape_system);
        return NULL;
    }
    TechnionFaculty j = CIVIL_ENGINEERING;
    for (int i = 0; i < escape_system -> num_of_faculties, j < UNKNOWN; 
                                                                i++, j++) {
        escape_system -> faculties[i] = FacultyCreate(j);
        if(escape_system -> faculties[i] == NULL) {
            ClearFaculties(escape_system, i);
            setDestroy(escape_system -> users);
            free(escape_system -> faculties);
            free(escape_system);
            return NULL;
        }
    }
    return escape_system;
}

/* This function is called when we want to add new company to the system. It
    gets escape_system, char* email and enum faculty and if the email doesnt
    exist in the system it adds the company with the wanted mail to the given
    faculty in the faculty arrays. If system or email or NULL or faculty is out
    of range or email conations more than 1 @ we return MTM_INVALID_PARAMETER.
    If email already exists we return MTM_EMAIL_ALREADY_EXISTS, and if there is
    memory problem we return MTM_OUT_OF_MEMORY, otherwise MTM_SUCCESS.*/
MtmErrorCode SystemAddCompany(EscapeTechnion escape_system, char* email, 
                                                TechnionFaculty faculty) {
    if(escape_system == NULL || email == NULL || (faculty >
                UNKNOWN || faculty < CIVIL_ENGINEERING) || 
                                    StringOccurencesOfChar(email, '@') != 1) {
        return MTM_INVALID_PARAMETER;
    }
    if(MailExists(escape_system, email)) {
        return MTM_EMAIL_ALREADY_EXISTS;
    }
    for(int i = 0; i < escape_system -> num_of_faculties; ++i) {
        if(FacultyGetName(escape_system -> faculties[i]) == faculty) {
            EscapeCompany company = CompanyCreate(email);
            if(company == NULL) {
                return MTM_OUT_OF_MEMORY;
            }
            MtmErrorCode insert_result = FacultyInsertCompany(escape_system -> 
                                                        faculties[i], company);
            if(insert_result != MTM_SUCCESS) {
                return insert_result;
            }
            return MTM_SUCCESS;
        }
    }
    return MTM_SUCCESS;
}

/* This function is called when we want to remove a company from the system. It
    gets escapse_system and char* email and removes the company with the given
    email if it exists. If escape_system or email are NULL or email is illegal 
    we return MTM_INVALID_PARAMETER, if company with that mail doesn't exist we 
    return MTM_COMPANY_EMAIL_DOES_NOT_EXIST, if the company has bookings we return MTM
    _RESERVATION_EXISTS, otherwise we return the result of the remove function*/
MtmErrorCode SystemRemoveCompany(EscapeTechnion escape_system, char* email) {
    if(escape_system == NULL || email == NULL || 
                        StringOccurencesOfChar(email, '@') != 1) {
            return MTM_INVALID_PARAMETER;                    
        }
    if(!CompanyMailExists(escape_system, email)) {
        return MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
    }
    Faculty faculty_of_company;
    EscapeCompany company_to_remove = GetCompany(escape_system, email, 
                                                        faculty_of_company);
    assert(company_to_remove != NULL); //Because we already checked it exists.
    if(CompanyHasBookings(company_to_remove)) {
        return MTM_RESERVATION_EXISTS;
    }
    MtmErrorCode remove_result = FacultyRemoveCompany(faculty_of_company, 
                                                        company_to_remove);
    return remove_result;
}

/* This function is called when we want to add company to the system. It gets
    escape_system, char* email, int id, int price, int num_ppl, char* working_
    hours and int difficulty. If one of the parameters is illegal (ints are out 
    of range or strings arent in right format or NULL) we return MTM_INVALD_
    PARAMETER, if company with the given mail doesn't exist we return 
    MTM_COMPANY_EMAIL_DOES_NOT_EXIST, if a room with the given ID already exists
    we return MTM_ID_ALREADY_EXISTS, otherwise returns the result of the company
    insert function. */
MtmErrorCode SystemAddRoom(EscapeTechnion escape_system, char* email, int id, 
                int price, int num_ppl, char* working_hours, int difficulty) {
    if(escape_system == NULL || email == NULL || StringOccurencesOfChar(email, 
        '@') != 1 || id <= 0 || price%4 != 0 || num_ppl <= 0 || difficulty < 1
        || difficulty > 10 || !CheckLegalHours(working_hours)) {
            return MTM_INVALID_PARAMETER;
    }
    if(!CompanyMailExists(escape_system, email)) {
        return MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
    }
    if(RoomIdExists(escape_system, id, email)) {
        return MTM_ID_ALREADY_EXIST;
    }
    EscapeCompany room_company = GetCompany(escape_system, email, NULL);
    EscapeRoom room_to_add = RoomCreate(id, price, num_ppl, working_hours,
                                                                    difficulty);
    MtmErrorCode insert_result = CompanyInsertRoom(room_company, room_to_add);
    return insert_result;
}