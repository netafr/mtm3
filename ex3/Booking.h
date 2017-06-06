#ifndef BOOKING_H
#define BOOKING_H
#include "set.h"
#include "list.h"
#include "mtm_ex3.h"
#include "User.h"

typedef struct booking_t *Booking;
Booking BookingCreate(int days, int hour, int total_price, int num_of_people, 
        char* user_email, char* company_email, TechnionFaculty room_faculty);
void BookingDestroy(void* booking);
void* BookingCopy(void* booking);
int BookingGetDays(Booking booking);
int BookingGetHour(Booking booking);
bool BookingUserBooking(Booking booking, char* email, int day, int hour);
int BookingCompare(void* booking1, void* booking2);
void BookingReduceDay(Booking booking);
#endif