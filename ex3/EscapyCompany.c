#include "EscapyCompany.h"
#include "EscapeRoom.h"
#include "Utils.h"
#include <malloc.h>
#include <assert.h>
#include <string.h>

struct escape_company_t {
    char* email;
    List company_rooms;//To sort
};

EscapeCompany CompanyCreate (char* email) {
    EscapeCompany company = malloc(sizeof(*company));
    if(company == NULL) {
        return NULL;
    }
    if (StringOccurencesOfChar(email, '@') != 1) {
        free(company);
        return NULL;
    }
    company -> email = StrDuplicate(email);
    if(company -> email == NULL) {
        free(company);
        return NULL;
    }
    company -> company_rooms = listCreate(RoomCopy, RoomDestroy);
    if(company -> company_rooms == NULL) {
        free(company -> email);
        free(company);
        return NULL;
    }
    return company;
}

void CompanyDestroy (void* company) {
    if(company == NULL) {
        return;
    }
    EscapeCompany temp = (EscapeCompany)company;
    listDestroy(temp -> company_rooms);
    free(temp -> email);
    free(temp);
}

void* CompanyCopy (void* company) {
    if(company == NULL) {
        return NULL;
    }
    EscapeCompany temp = (EscapeCompany)company;
    EscapeCompany new_company = malloc(sizeof(*new_company));
    if(new_company == NULL) {
        return NULL;
    }
    new_company -> email = StrDuplicate(temp -> email);
    if(new_company -> email == NULL) {
        free(new_company);
        return NULL;
    }
    new_company -> company_rooms = listCopy(temp -> company_rooms);
    if(new_company -> company_rooms == NULL) {
        free(new_company -> email);
        free(new_company);
        return NULL;
    }
    return (void*)new_company;
}

int CompanyCompare(void* company1, void* company2) {
    assert(company1 != NULL && company2 != NULL);
    EscapeCompany temp1 = (EscapeCompany)company1;
    EscapeCompany temp2 = (EscapeCompany)company2;
    return (strcmp(temp1 -> email, temp2 -> email));
}

char* CompanyGetEmail(EscapeCompany company) {
    if(company == NULL) {
        return NULL;
    }
    return (company -> email);
}

bool CompanyHasBookings(EscapeCompany company) {
    if(company == NULL) {
        return false;
    }
    EscapeRoom curr_room;
    LIST_FOREACH(EscapeRoom, curr_room, company -> company_rooms) {
        if(RoomHasBookings(curr_room)) {
            return true;
        }
    }
    return false;
}

bool CompanyRoomExists(EscapeCompany company, int id) {
    assert(company != NULL && id > 0);
    EscapeRoom curr_room;
    LIST_FOREACH(EscapeRoom, curr_room, company -> company_rooms) {
        if(RoomGetId(curr_room) == id) {
            return true;
        }
    }
    return false;
}

MtmErrorCode CompanyInsertRoom(EscapeCompany company, EscapeRoom room) {
    assert(company != NULL && room != NULL);
    ListResult insert_result = listInsertLast(company -> company_rooms, 
                                                                (void*)room);
    if(insert_result == LIST_OUT_OF_MEMORY) {
        return MTM_OUT_OF_MEMORY;
    }
    return MTM_SUCCESS;
}