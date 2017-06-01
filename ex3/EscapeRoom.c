#include "EscapeRoom.h"
#include "Utils.h"
#include "mtm_ex3.h"
#include "Booking.h"
#include <malloc.h>
#include <assert.h>

struct escape_room_t {
    int id;
    int price_for_person;
    int recommended_num_person;
    char *working_hours;
    int difficulty;
    List bookings; //To sort
};

EscapeRoom RoomCreate(int id, int price, int num_person, char* working_hours, 
                        int difficulty) {
    EscapeRoom room = malloc(sizeof(*room));
    if(room == NULL) {
        return NULL;
    }
    room -> id = id;
    room -> price_for_person = price;
    room -> recommended_num_person = num_person;
    room -> difficulty = difficulty;
    room -> working_hours = StrDuplicate(working_hours);
    if(room -> working_hours == NULL) {
        free(room);
        return NULL;
    }
    room -> bookings = listCreate(BookingCopy, BookingDestroy);
    if(room -> bookings == NULL) {
        free(room -> working_hours);
        free(room);
        return NULL;
    }
    return room;
}

void RoomDestroy(void* room) {
    if(room == NULL) {
        return;
    }
    EscapeRoom temp = (EscapeRoom)room;
    free(temp -> working_hours);
    listDestroy(temp -> bookings);
    free(temp);
}

void* RoomCopy(void* room) {
    if(room == NULL) {
        return NULL;
    }
    EscapeRoom temp = (EscapeRoom)room;
    EscapeRoom new_room = malloc(sizeof(*new_room));
    if(new_room == NULL) {
        return NULL;
    }
    new_room -> id = temp -> id;
    new_room -> price_for_person = temp -> price_for_person;
    new_room -> recommended_num_person = temp -> recommended_num_person;
    new_room -> difficulty = temp -> difficulty;
    new_room -> working_hours = StrDuplicate(temp -> working_hours);
    if(new_room -> working_hours == NULL) {
        free(new_room);
        return NULL;
    }
    new_room -> bookings = listCopy(temp -> bookings);
    if(new_room -> bookings == NULL) {
        free(new_room -> working_hours);
        free(new_room);
        return NULL;
    }
    return (void*)new_room;
}

bool RoomHasBookings(EscapeRoom room) {
    if(room == NULL) {
        return false;
    }
    if(listGetSize > 0) {
        return true;
    }
    return false;
}

int RoomGetId(EscapeRoom room) {
    if(room == NULL) {
        return 0;
    }
    return room -> id;
}