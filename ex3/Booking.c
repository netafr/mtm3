#include "Booking.h"
#include <malloc.h>
#include <string.h>
#include "mtm_ex3.h"

struct booking_t {
    int days;
    int hour;
    int total_price;
    int num_of_people;
    char* user_email;
    char* company_email;
    TechnionFaculty room_faculty;
    int room_id;
};
/* Creates a new booking. Receives the day of the booking, hour of the booking,
   the price, the number of people involved, the user email, the company email,
   the faculty the room belongs to, and the room ID. 
   Creates a new booking session and returns NULL if one of the variables are 
   illegal. */
Booking BookingCreate(int days, int hour, int total_price, int num_of_people, 
        char* user_email, char* company_email, TechnionFaculty room_faculty,    
                                                                int room_id) {
    if(days < 0 || hour > 23 || hour < 0 || num_of_people < 0 || user_email == 
        NULL || company_email == NULL || room_faculty < 0 || room_faculty > 
            UNKNOWN || StringOccurencesOfChar(user_email, '@') != 1 ||
                StringOccurencesOfChar(company_email, '@') != 1) {
        return NULL;            
    }
    Booking booking = malloc(sizeof(*booking));
    if(booking == NULL) {
        return NULL;
    }
    booking -> days = days;
    booking -> hour = hour;
    booking -> total_price = total_price;
    booking -> num_of_people = num_of_people;
    booking -> room_faculty = room_faculty;
    booking -> room_id = room_id;
    booking -> user_email = StrDuplicate(user_email);
    if(booking -> user_email == NULL) {
        free(booking);
        return NULL;
    }
    booking -> company_email = StrDuplicate(company_email);
    if(booking -> company_email == NULL) {
        free(booking -> user_email);
        free(booking);
        return NULL;
    }
    return booking;
}

void BookingDestroy(void* booking) {
    if(booking == NULL) {
        return;
    }
    Booking temp = (Booking)booking;
    free(temp -> user_email);
    free(temp -> company_email);
    free(temp);
}

void* BookingCopy(void* booking) {
    if(booking == NULL) {
        return NULL;
    }
    Booking temp = (Booking)booking;
    Booking new_booking = malloc(sizeof(*new_booking));
    if(new_booking == NULL) {
        return NULL;
    }
    new_booking -> days = temp -> days;
    new_booking -> hour = temp -> hour;
    new_booking -> total_price = temp -> total_price;
    new_booking -> num_of_people = temp -> num_of_people;
    new_booking -> room_faculty = temp -> room_faculty;
    new_booking -> room_id = temp -> room_id;
    new_booking -> user_email = StrDuplicate(temp -> user_email);
    if(new_booking -> user_email == NULL) {
        free(new_booking);
        return NULL;
    }
    new_booking -> company_email = StrDuplicate(temp -> company_email);
    if(new_booking -> company_email == NULL) {
        free(new_booking -> user_email);
        free(new_booking);
        return NULL;
    }
    return (void*)new_booking;
}

int BookingGetDays(Booking booking) {
    if(booking == NULL) {
        return -1;
    }
    return booking -> days;
}

int BookingGetHour(Booking booking) {
    if(booking == NULL) {
        return -1;
    }
    return booking -> hour;
}

bool BookingUserBooking(Booking booking, char* email, int hour, int day) {
    if(booking == NULL || email == NULL) {
        return false;
    }
    if(strcmp(booking -> user_email, email) == 0 && hour == booking -> hour &&
                                                    day == booking -> days) {
        return true;
    }
    return false;
}

int BookingCompare(void* booking1, void* booking2) {
    assert(booking1 != NULL && booking2 != NULL);
    Booking new_booking1 = (Booking)booking1, new_booking2 = (Booking)booking2;
    return(new_booking1 -> days - new_booking2 -> days);
}

void BookingReduceDay(Booking booking) {
    if(booking == NULL) {
        return;
    }
    (booking -> days)--;
}

TechnionFaculty BookingGetFaculty(Booking booking) {
    if(booking == NULL) {
        return -1;
    }
    return booking -> room_faculty;
}

char* BookingGetUserEmail(Booking booking) {
    if(booking == NULL) {
        return NULL;
    }
    return booking -> user_email;
}


char* BookingGetCompanyEmail(Booking booking) {
    if(booking == NULL) {
        return NULL;
    }
    return booking -> company_email;
}

int BookingGetNumPpl(Booking booking) {
    if(booking == NULL) {
        return -1;
    }
    return booking -> num_of_people;
}

int BookingGetPrice(Booking booking) {
    if(booking == NULL) {
        return -1;
    }
    return booking -> total_price;
}

int BookingGetRoomId(Booking booking) {
    if(booking == NULL) {
        return -1;
    }
    return booking -> room_id;
}