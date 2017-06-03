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
    LIST_FOREACH(EscapeRoom, curr_room, company -> company_rooms) {
        if(RoomHasBookings(curr_room)) {
            return true;
        }
    }
    return false;
}

bool CompanyRoomExists(EscapeCompany company, int id) {
    assert(company != NULL && id > 0);
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

MtmErrorCode CompanyRemoveRoom(EscapeCompany company, EscapeRoom room) {
    assert(company != NULL && room != NULL);
    LIST_FOREACH(EscapeRoom, curr_room, company -> company_rooms) {
        if(curr_room == room) {//Both are pointers, check if they point on same.
            ListResult remove_result = listRemoveCurrent(company -> 
                                                                company_rooms);
            if(remove_result == LIST_INVALID_CURRENT) {
                return MTM_OUT_OF_MEMORY;
            }
            return MTM_SUCCESS;
        }
    }
    return MTM_SUCCESS;
}

EscapeRoom CompanyGetRoom(EscapeCompany company, int id) {
    assert(company != NULL && id > 0);
    LIST_FOREACH(EscapeRoom, curr_room, company -> company_rooms) {
        if(RoomGetId(curr_room) == id) {
            return curr_room;
        }
    }
    return NULL;
}

bool CompanyUserHasBookings(EscapeCompany company, char* email, int hour, 
                                                                    int day) {
    assert(company != NULL && email != NULL);
    LIST_FOREACH(EscapeRoom, curr_room, company -> company_rooms) {
        if(RoomUserHasBookings(curr_room, email, hour, day)) {
            return true;
        }
    }
    return false;
}

bool CompanyHasRooms(EscapeCompany company) {
    if(company == NULL) {
        return false;
    }
    if(listGetSize(company -> company_rooms) < 1) {
        return false;
    }
    return true;
}

EscapeRoom CompanyGetRecommendedRoom(EscapeCompany company, int level, int
                                                num_ppl, int* id, int* score) {
    if(company == NULL) {
        return NULL;
    }
    EscapeRoom minRoom = listGetFirst(company -> company_rooms);
    int minScore = RoomGetScore(minRoom, level, num_ppl);
    int minId = RoomGetId(minRoom);
    int tempScore, tempId;
    LIST_FOREACH(EscapeRoom, curr_room, company -> company_rooms) {
        tempScore = RoomGetScore(curr_room, level, num_ppl);
        tempId = RoomGetId(curr_room);
        if(tempScore < minScore) {
            minScore = tempScore;
            minId = tempId;
            minRoom = curr_room;
        } else if(tempScore == minScore && tempId < minId) {
            minScore = tempScore;
            minId = tempId;
            minRoom = curr_room;
        }
    }
    *(id) = minId;
    *(score) = minScore;
    return minRoom;
}