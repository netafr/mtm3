#include "test_utilities.h"
#include "../Booking.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "../mtm_ex3.h"

static bool testBookingCreate() {
    ASSERT_TEST(BookingCreate(1, 2, 4, 4, NULL, "das@3131", 1, 6) == NULL);
    ASSERT_TEST(BookingCreate(1, 2, 4, 4, "das@3131", NULL, 1, 6) == NULL);
    ASSERT_TEST(BookingCreate(1, 2, 4, 4, "3131", "41@gfaa5", 1, 6) == NULL);
    ASSERT_TEST(BookingCreate(1, 2, 4, 4, "das@3131", "g", 1, 6) == NULL);
    Booking booking = BookingCreate(1, 2, 4, 4, "das@3131", "41@gfaa", 1, 6);
    ASSERT_TEST(booking != NULL);
    BookingDestroy(booking);
    return true;
}

static bool testBookingDestroy() {
    BookingDestroy(NULL);
    Booking booking = BookingCreate(1, 2, 4, 4, "das@3131", "41@gfaa", 1, 6);
    ASSERT_TEST(booking != NULL);
    BookingDestroy(booking);
    return true;
}

static bool testBookingCopy() {
    Booking copyFrom = BookingCreate(1, 2, 4, 4, "das@3131", "41@gfaa", 1, 6);
    Booking copyTo = BookingCopy(copyFrom);
    ASSERT_TEST(copyTo != NULL);
    ASSERT_TEST(BookingGetHour(copyFrom) == BookingGetHour(copyTo));
    ASSERT_TEST(strcmp(BookingGetCompanyEmail(copyFrom), 
                                        BookingGetCompanyEmail(copyTo)) == 0);
    BookingDestroy(copyFrom);
    BookingDestroy(copyTo);
    return true;
}

static bool testBookingGetDays() {
    Booking booking = BookingCreate(5, 2, 4, 4, "das@3131", "41@gfaa", 1, 6);
    ASSERT_TEST (BookingGetDays(booking) == 5);
    BookingDestroy(booking);
    return true;
}

static bool testBookingGetHour() {
    Booking booking = BookingCreate(5, 2, 4, 4, "das@3131", "41@gfaa", 1, 6);
    ASSERT_TEST (BookingGetHour(booking) == 2);
    BookingDestroy(booking);
    return true;
}

static bool testBookingUserBooking() {
    Booking booking = BookingCreate(1, 2, 4, 4, "usern@3131", "41@comp", 1, 6);
    char* str = "usern@3131";
    ASSERT_TEST(BookingUserBooking(booking, str, 2, 1) == true);
    ASSERT_TEST(BookingUserBooking(booking, str, 0, 1) == false);
    ASSERT_TEST(BookingUserBooking(booking, str, 2, 3) == false);
    str = "usera@3131";
    ASSERT_TEST(BookingUserBooking(booking, str, 2, 1) == false);
    BookingDestroy(booking);
    return true;
}

static bool testBookingCompare() {
    Booking a = BookingCreate(1, 2, 4, 4, "usern@1131", "21@comp", 1, 4);
    Booking b = BookingCreate(2, 1, 4, 4, "usern@2131", "31@comp", 2, 5);
    Booking c = BookingCreate(2, 2, 4, 4, "usern@3131", "41@comp", 3, 6);
    ASSERT_TEST(BookingCompare(a, b) < 0);
    ASSERT_TEST(BookingCompare(c, a) > 0);
    ASSERT_TEST(BookingCompare(b, c) == 0);
    BookingDestroy(a);
    BookingDestroy(b);
    BookingDestroy(c);
    return true;
}

static bool testBookingReduceDay() {
    Booking a = BookingCreate(1, 2, 4, 4, "usern@1131", "21@comp", 1, 4);
    Booking b = BookingCreate(2, 1, 4, 4, "usern@2131", "31@comp", 2, 5);
    BookingReduceDay(a);
    BookingReduceDay(b);
    ASSERT_TEST(BookingGetDays(a) == 0);
    ASSERT_TEST(BookingGetDays(b) == 1);
    BookingDestroy(a);
    BookingDestroy(b);
    return true;
}

static bool testBookingGetFaculty() {
    ASSERT_TEST(BookingGetFaculty(NULL) == -1);
    Booking a = BookingCreate(1, 2, 4, 4, "usern@1131", "21@comp", 1, 4);
    ASSERT_TEST(BookingGetFaculty(a) == 1);
    BookingDestroy(a);
    return true;
}

static bool testBookingGetUserEmail() {
    ASSERT_TEST(BookingGetUserEmail(NULL) == NULL);
    Booking a = BookingCreate(1, 2, 4, 4, "usern@1131", "21@comp", 1, 4);
    ASSERT_TEST(strcmp(BookingGetUserEmail(a), "usern@1131") == 0);
    BookingDestroy(a);
    return true;
}

static bool testBookingGetCompanyEmail() {
    ASSERT_TEST(BookingGetCompanyEmail(NULL) == NULL);
    Booking a = BookingCreate(1, 2, 4, 4, "usern@1131", "21@comp", 1, 4);
    ASSERT_TEST(strcmp(BookingGetCompanyEmail(a), "21@comp") == 0);
    BookingDestroy(a);
    return true;
}

static bool testBookingGetNumPpl() {
    ASSERT_TEST(BookingGetNumPpl(NULL) == -1);
    Booking a = BookingCreate(1, 2, 4, 4, "usern@1131", "21@comp", 1, 4);
    ASSERT_TEST(BookingGetNumPpl(a) == 4);
    BookingDestroy(a);
    return true;
}

static bool testBookingGetPrice() {
    ASSERT_TEST(BookingGetPrice(NULL) == -1);
    Booking a = BookingCreate(1, 2, 3, 4, "usern@1131", "21@comp", 1, 4);
    ASSERT_TEST(BookingGetPrice(a) == 3);
    BookingDestroy(a);
    return true;
}

static bool testBookingGetRoomId() {
    ASSERT_TEST(BookingGetRoomId(NULL) == -1);
    Booking a = BookingCreate(1, 2, 3, 4, "usern@1131", "21@comp", 1, 10);
    ASSERT_TEST(BookingGetRoomId(a) == 10);
    BookingDestroy(a);
    return true;
}

int main() {
    RUN_TEST(testBookingCreate);
    RUN_TEST(testBookingDestroy);
    RUN_TEST(testBookingCopy);
    RUN_TEST(testBookingGetDays);
    RUN_TEST(testBookingGetHour);
    RUN_TEST(testBookingUserBooking);
    RUN_TEST(testBookingCompare);
    RUN_TEST(testBookingReduceDay);
    RUN_TEST(testBookingGetFaculty);
    RUN_TEST(testBookingGetUserEmail);
    RUN_TEST(testBookingGetCompanyEmail);
    RUN_TEST(testBookingGetNumPpl);
    RUN_TEST(testBookingGetPrice);
    RUN_TEST(testBookingGetRoomId);
    printf("Done! \n");
    return 0;
}