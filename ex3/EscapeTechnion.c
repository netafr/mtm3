#include "EscapeTechnion.h"
#include "mtm_ex3.h"
#include "EscapyCompany.h"
#include "set.h"
#include "User.h"
#include "Faculty.h"
#include "Booking.h"
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

/* Helper function that gets escape_system and char* email and returns a pointer
    to the user with the given email. If doesn't exist we return NULL. */
static User GetUser(EscapeTechnion escape_system, char* email) {
    assert(escape_system != NULL && email != NULL);
    SET_FOREACH(User, curr_user, escape_system -> users) {
        if(strcmp(UserGetEmail(curr_user), email) == 0) {
            return curr_user;
        }
    }
    return NULL;
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
    if(setIsIn(escape_system -> users, (void*)GetUser(escape_system, email))) {
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

/* Helper function that gets escape_system, int id, TechnionFaculty faculty and
   EscapeCompany company, and returns pointer to the room in the given faculty 
   with the given id, if there is an error for some reason we return NULL.
   company is optional, if given it will store the pointer to the room's company
   */
static EscapeRoom GetRoom(EscapeTechnion escape_system, int id, TechnionFaculty 
                                            faculty ,EscapeCompany company) {
    assert(escape_system != NULL);
    EscapeRoom room;
    room = FacultyGetRoom(escape_system -> faculties[faculty], id, company);
    if(room != NULL) {
        return room;
    }
    return NULL;
}

/* This function gets escape_system, char* email int hour and int day and return
    whether the user has a booking in the same day and hour. */
bool UserHasBookings(EscapeTechnion escape_system, char* email, int hour, 
                                                                int day) {
    assert(escape_system != NULL && email != NULL);
    for(int i = 0; i < escape_system -> num_of_faculties; ++i) {
        if(FacultyUserHasBookings(escape_system -> faculties[i], email, hour,
                                                                        day)) {
            return true;
        }
    }
    return false;
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
            CompanyDestroy(company); //Set creates copy so we free the original.
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

/* This function is called when we want to add room to the system. It gets
    escape_system, char* email, int id, int price, int num_ppl, char* working_
    hours and int difficulty. If one of the parameters is illegal (ints are out 
    of range or strings arent in right format or NULL) we return MTM_INVALD_
    PARAMETER, if company with the given mail doesn't exist we return 
    MTM_COMPANY_EMAIL_DOES_NOT_EXIST, if a room with the given ID already exists
    we return MTM_ID_ALREADY_EXISTS, otherwise returns the result of the room
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
    RoomDestroy(room_to_add); //List adds a copy so we free the original.
    return insert_result;
}

/* This function is called when we want to remove room from the system. It gets
    escape_system, TechnionFaculty faculty and int id. If escapse_system is NULL
    or id or faculty are out of range, we return MTM_INVALID_PARAMETER, if room
    with the given id doesn't exists we return MTM_ID_DOES_NOT_EXIST, if the 
    room to remove has bookings we return MTM_RESERVATION_EXIST, otherwise we
    return the result of the CompanyRemoveRoom function. */
MtmErrorCode SystemRemoveRoom(EscapeTechnion escape_system, TechnionFaculty 
                        faculty, int id) {
    if(escape_system == NULL || id < 1 || faculty < CIVIL_ENGINEERING || faculty
                                                                    > UNKNOWN){
        return MTM_INVALID_PARAMETER;
    }
    EscapeCompany room_company;
    EscapeRoom wanted_room = GetRoom(escape_system, id, faculty, room_company);
    if(wanted_room == NULL) {
        return MTM_ID_DOES_NOT_EXIST;
    }
    if(RoomHasBookings(wanted_room)) {
        return MTM_RESERVATION_EXISTS;
    }
    MtmErrorCode remove_result = CompanyRemoveRoom(room_company, wanted_room);
    return remove_result;
}

/* This function is called when we want to add new user to the system. It gets
    escape_system, char* email, TechnionFaculty faculty and int skill_level. If
    escape_system or email are NULL, or email, faculty and skill_level are 
    illegal we return MTM_INVALID_PARAMETER, if something with the given email
    already exists in the system we return MTM_EMAIL_ALREADY_EXISTS, if user 
    creation or copy fails we return MTM_OUT_OF_MEMORY, otherwise we return
    MTM_SUCCESS. */
MtmErrorCode SystemAddUser(EscapeTechnion escape_system, char* email, 
                                TechnionFaculty faculty , int skill_level) {
    if(escape_system == NULL || email == NULL || StringOccurencesOfChar(email, 
            '@') != 1 || faculty < CIVIL_ENGINEERING || faculty > UNKNOWN || 
                                        skill_level < 1 || skill_level > 10){
        return MTM_INVALID_PARAMETER;                                
    }
    if(MailExists(escape_system, email)) {
        return MTM_EMAIL_ALREADY_EXISTS;
    }
    User user = UserCreate(email, skill_level, faculty);
    if(user == NULL) {
        return MTM_OUT_OF_MEMORY;
    }
    SetResult insert_result = setAdd(escape_system -> users, (void*)user);
    UserDestroy(user); //Set adds a copy so we clean the original.
    if(insert_result != SET_SUCCESS) {
        return MTM_OUT_OF_MEMORY;
    }
    return MTM_SUCCESS;
}

/* This function is called when we want to remove user from the system. It gets
    escape_system and char* email. If escape system or email are NULL or email
    is illegal we return MTM_INVALID_PARAMETER, if the user doesn't exist we 
    return MTM_CLIENT_EMAIL_DOES_NOT_EXIST, if setRemove fails we return MTM_
    OUT_OF_MEMORY, otherwise we return MTM_SUCCESS. */
MtmErrorCode SystemRemoveUser(EscapeTechnion escape_system, char* email) {
    if(escape_system == NULL || email == NULL || StringOccurencesOfChar(email,
                                                                '@' != 1)) {
        return MTM_INVALID_PARAMETER;
    }
    User user = GetUser(escape_system, email);
    if(user == NULL) {
        return MTM_CLIENT_EMAIL_DOES_NOT_EXIST;
    }
    SetResult remove_result = setRemove(escape_system -> users, (void*)user);
    if(remove_result != SET_SUCCESS) {
        return MTM_OUT_OF_MEMORY;
    }
    return MTM_SUCCESS;
}

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
                TechnionFaculty faculty, int id, char* time, int num_ppl) {
    if(escape_system == NULL || user_email == NULL || StringOccurencesOfChar
    (user_email, '@') != 1 || id < 1 || num_ppl < 1 || !CheckLegalDayTime(time))
                                                                            {
        return MTM_INVALID_PARAMETER;
    }
    User user = GetUser(escape_system, user_email);
    if(user == NULL) {
        return MTM_CLIENT_EMAIL_DOES_NOT_EXIST;
    }
    EscapeCompany company;
    EscapeRoom room = GetRoom(escape_system, id, faculty, company);
    if(room == NULL) {
        return MTM_ID_DOES_NOT_EXIST;
    }
    int day, hour;
    GetTimes(time, &day, &hour);
    if(UserHasBookings(escape_system, user_email, hour, day)) {
        return MTM_CLIENT_IN_ROOM;
    }
     if(!RoomAvailable(room, day, hour)) {
        return MTM_ROOM_NOT_AVAILABLE;
    }
    int price;
    if(faculty == UserGetFaculty(user)) {
        price =  (3*(RoomGetPrice(room) / 4)) * num_ppl;
    } else {
         price = RoomGetPrice(room) * num_ppl;
    }
    Booking booking = BookingCreate(day, hour, price, num_ppl, user_email, 
                                            CompanyGetEmail(company), faculty);
    if(booking == NULL) {
        return MTM_OUT_OF_MEMORY;
    }
    MtmErrorCode insert_result = RoomAddBooking(room, booking);
    return insert_result;
}