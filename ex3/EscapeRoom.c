#include "EscapeRoom.h"
#include "Utils.h"
#include "mtm_ex3.h"
#include "Booking.h"
#include <malloc.h>
#include <assert.h>
#include <math.h>

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
    if(id < 0 || price%4 != 0 || num_person <= 0 || difficulty < 1 || difficulty
                                > 10 || working_hours == NULL || 
                                CheckLegalHours(working_hours) != TRUE) {
        return NULL;                                
    } 
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

int RoomGetPrice(EscapeRoom room) {
    if(room == NULL) {
        return -1;
    }
    return room -> price_for_person;
}

bool RoomUserHasBookings(EscapeRoom room, char *email, int hour, int day) {
    if(room == NULL || email == NULL) {
        return false;
    }
    LIST_FOREACH(Booking, curr_booking, room -> bookings) {
        if(BookingUserBooking(curr_booking, email, hour, day)) {
            return true;
        }
    }
    return false;
}

bool RoomAvailable(EscapeRoom room, int day, int hour) {
    if(room == NULL || day < 0 || hour < 0 || hour > 23) {
        return false;
    }
    int opening, closing;
    GetTimes(room -> working_hours, &opening, &closing);
    if(hour < opening || hour >= closing) {
        return false;
    }
    LIST_FOREACH(Booking, curr_booking, room -> bookings) {
        if(BookingGetDays(curr_booking) == day && BookingGetHour(curr_booking) 
                                                                    == hour) {
            return false;                                                            
        }
    }
    return true;
}

RoomErr RoomAddBooking(EscapeRoom room, Booking booking) {
    if(room == NULL || booking == NULL) {
        return ROOM_INVALID_PARAMETER;
    }
    ListResult insert_result = listInsertLast(room -> bookings, (void*)booking);
    if(insert_result == LIST_OUT_OF_MEMORY) {
        return ROOM_OUT_OF_MEMORY;
    } else if(insert_result == LIST_NULL_ARGUMENT) {
        return ROOM_INVALID_PARAMETER;
    }
    ListResult sort_result = listSort(room -> bookings, BookingCompare);
    if(sort_result == LIST_OUT_OF_MEMORY) {
        return ROOM_OUT_OF_MEMORY;
    } else if(sort_result == LIST_NULL_ARGUMENT) {
        return ROOM_INVALID_PARAMETER;
    }
    return ROOM_SUCCESS;
}

int RoomGetScore(EscapeRoom room, int level, int num_ppl) {
    if(room == NULL) {
        return -1;
    }
    return sqrt(pow(room -> recommended_num_person - num_ppl, 2) + pow(room -> 
                                                        difficulty - level, 2));
}

char* RoomGetClosestAvailable(EscapeRoom room) {
    if(room == NULL) {
        return NULL;
    }
    int day = 0, hour = 0;
    while(!RoomAvailable(room, day, hour)) {
        if(hour == 23) {
            day++;
            hour = 0;
        } else {
            hour++;
        }
    }
    char* time = CreateString(hour, day);
    return time;
}

int RoomCompare(void* room1, void* room2) {
    assert(room1 != NULL && room2 != NULL);
    EscapeRoom new_room1 = (EscapeRoom)room1, new_room2 = (EscapeRoom)room2;
    return(new_room1 -> id - new_room2 -> id);
}

List RoomGetTodayList(EscapeRoom room) {
    if(room == NULL) {
        return NULL;
    }
    List list = listCreate(BookingCopy, BookingDestroy);
    if(list == NULL) {
        return NULL;
    }
    LIST_FOREACH(Booking, curr_booking, room -> bookings) {
        if(BookingGetDays(curr_booking) == 0) {
            ListResult list_insert = listInsertFirst(list, curr_booking);
            if(list_insert == LIST_OUT_OF_MEMORY) {
                return NULL;
            } //NULL arguemnt won't happen
        } else {
            BookingReduceDay(curr_booking);
        }
    }
    return list;
}