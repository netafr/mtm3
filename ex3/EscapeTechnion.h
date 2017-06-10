#ifndef ESCAPE_TECHNION_H
#define ESCAPE_TECHNION_H
#include "set.h"
#include "list.h"
#include "Faculty.h"

typedef struct escape_technion_t *EscapeTechnion;

/* This function creates the system upon initializtion, when the program is 
    called. Its mallocs and creates the faculties array and the users set. 
    It initliazes all the faculties. If there is a memory problem it returns
    NULL, otherwise returns the newly created system. */
EscapeTechnion SystemCreate();

/* This function is called when we want to add new company to the system. It
    gets escape_system, char* email and enum faculty and if the email doesnt
    exist in the system it adds the company with the wanted mail to the given
    faculty in the faculty arrays. If system or email or NULL or faculty is out
    of range or email conations more than 1 @ we return MTM_INVALID_PARAMETER.
    If email already exists we return MTM_EMAIL_ALREADY_EXISTS, and if there is
    memory problem we return MTM_OUT_OF_MEMORY, otherwise MTM_SUCCESS.*/
MtmErrorCode SystemAddCompany(EscapeTechnion escape_system, char* email, 
                                                TechnionFaculty faculty);
                                                
/* This function is called when we want to remove a company from the system. It
    gets escapse_system and char* email and removes the company with the given
    email if it exists. If escape_system or email are NULL or email is illegal 
    we return MTM_INVALID_PARAMETER, if company with that mail doesn't exist we 
    return MTM_COMPANY_EMAIL_DOES_NOT_EXIST, if the company has bookings we 
    return MTM _RESERVATION_EXISTS, otherwise we return the result of the remove
    function*/
MtmErrorCode SystemRemoveCompany(EscapeTechnion escape_system, char* email);

/* This function is called when we want to add room to the system. It gets
    escape_system, char* email, int id, int price, int num_ppl, char* working_
    hours and int difficulty. If one of the parameters is illegal (ints are out 
    of range or strings arent in right format or NULL) we return MTM_INVALD_
    PARAMETER, if company with the given mail doesn't exist we return 
    MTM_COMPANY_EMAIL_DOES_NOT_EXIST, if a room with the given ID already exists
    we return MTM_ID_ALREADY_EXISTS, otherwise returns the result of the room
    insert function. */
MtmErrorCode SystemAddRoom(EscapeTechnion escape_system, char* email, int id, 
                int price, int num_ppl, char* working_hours, int difficulty);
                
/* This function is called when we want to remove room from the system. It gets
    escape_system, TechnionFaculty faculty and int id. If escapse_system is NULL
    or id or faculty are out of range, we return MTM_INVALID_PARAMETER, if room
    with the given id doesn't exists we return MTM_ID_DOES_NOT_EXIST, if the 
    room to remove has bookings we return MTM_RESERVATION_EXIST, otherwise we
    return the result of the CompanyRemoveRoom function. */
MtmErrorCode SystemRemoveRoom(EscapeTechnion escape_system, TechnionFaculty 
                        faculty, int id);

/* This function is called when we want to add new user to the system. It gets
    escape_system, char* email, TechnionFaculty faculty and int skill_level. If
    escape_system or email are NULL, or email, faculty and skill_level are 
    illegal we return MTM_INVALID_PARAMETER, if something with the given email
    already exists in the system we return MTM_EMAIL_ALREADY_EXISTS, if user 
    creation or copy fails we return MTM_OUT_OF_MEMORY, otherwise we return
    MTM_SUCCESS. */
MtmErrorCode SystemAddUser(EscapeTechnion escape_system, char* email, 
                                TechnionFaculty faculty , int skill_level);
                                
/* This function is called when we want to remove user from the system. It gets
    escape_system and char* email. If escape system or email are NULL or email
    is illegal we return MTM_INVALID_PARAMETER, if the user doesn't exist we 
    return MTM_CLIENT_EMAIL_DOES_NOT_EXIST, if setRemove fails we return MTM_
    OUT_OF_MEMORY, otherwise we return MTM_SUCCESS. */
MtmErrorCode SystemRemoveUser(EscapeTechnion escape_system, char* email);

/* This function is called when we want to create a booking for an user at a
    given room, according to the given date and number of people. It gets 
    escape_system, char* user_email, TechnionFaculty faculty, int id (of room),
    char* time and int num_ppl. If escape_system or user_email are NULL or one
    of the parameters isn't in legal range or format we return MTM_INVALID_
    PARMETER, if a user with the given email doesn't exist we return MTM_CLIENT_
    EMAIL_DOES_NOT_EXIST, if a room with the given id doesn't exist, we return
    MTM_ID_DOES_NOT_EXIST, if the user already has a booking in the given time
    we return MTM_CLIENT_IN_ROOM, if there is already a booking for the given
    time in the wanted room we return MTM_ROOM_NOT_AVAILABLE, if there is a 
    memory problem we return MTM_OUT_OF_MEMORY, otherwise we return MTM_SUCCESS.
    */
MtmErrorCode SystemAddOrder(EscapeTechnion escape_system, char* user_email, 
                TechnionFaculty faculty, int id, char* time, int num_ppl);
                
/* This function is called when want to get the recommended room for a user and
    make a booking for the closest time in it. It gets escape_system, char* 
    email and int num_ppl. If escape_system or email are NULL or email or 
    num_ppl are illegal we return MTM_INVALID_PARAMETER, if the user with the 
    given email doesn't exist we return MTM_CLIENT_EMAIL_DOES_NOT_EXIST, if the
    system doesn't have rooms we return MTM_NO_ROOMS_AVAILABLE, if there is
    memory error along the way we return MTM_OUT_OF_MEMORY, otherwise we return
    the result of the SystemAddBooking function. */
MtmErrorCode SystemRecommendedRoom(EscapeTechnion escape_system, char* email, 
                                                            int num_ppl);

/* This function is called when we want to end the current day, make the booking
    payments for today, write today's events and update the whole system. It 
    gets escape_system and FILE* outputChannel. If one the parameters are NULL
    we return MTM_INVALID_PARMETER, if there is an error during getting the 
    today's list fails we return MTM_MEMORY_PROBLEM, otherwise we return
    MTM_SUCCESS. */
MtmErrorCode SystemReportDay(EscapeTechnion escape_system, FILE* outputChannel);

/* This function is called when we want to report the top 3 faculties. It gets
    escape_system and FILE* outputChannel, and if they are not NULL it prints
    the top3 faculties, total revenue and each of the top faculties' revenue. */
void SystemReportBest(EscapeTechnion escape_system, FILE* outputChannel);

/* This function is called when we want to end the program. It gets 
    escape_system and frees all the malloc'ed memory. */
void SystemDestroy(EscapeTechnion escape_system);
#endif