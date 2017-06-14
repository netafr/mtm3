#include "test_utilities.h"
#include "../EscapeRoom.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "../mtm_ex3.h"


static bool RoomsEqual(EscapeRoom room1, EscapeRoom room2) {
    assert(room1 != NULL && room2 != NULL);
    if(RoomGetId(room1) != RoomGetId(room2)) {
        return false;
    }
    if(RoomGetPrice(room1) != RoomGetPrice(room2)) {
        return false;
    }
    if(RoomGetDifficulty(room1) != RoomGetDifficulty(room2)) {
        return false;
    }
    return true;
}

static bool testRoomCreate(){
    ASSERT_TEST(RoomCreate(1, 4, 2, NULL, 3) == NULL);
    ASSERT_TEST(RoomCreate(2, 4, 2, "12-8", 3) == NULL);
    ASSERT_TEST(RoomCreate(3, 4, 2, "12-833", 3) == NULL);
    EscapeRoom room = RoomCreate(1, 8, 3, "15-17", 3);
    ASSERT_TEST(room != NULL);
    RoomDestroy(room);
    return true;
}

static bool testRoomDestroy() {
    RoomDestroy(NULL);
    EscapeRoom room = RoomCreate(1, 8, 3, "15-17", 3);
    ASSERT_TEST(room != NULL);
    RoomDestroy(room);
    return true;
}

static bool testRoomCopy() {
    ASSERT_TEST(RoomCopy(NULL) == NULL);
    EscapeRoom room = RoomCreate(1, 8, 3, "15-17", 3);
    EscapeRoom copy = RoomCopy(room);
    ASSERT_TEST(copy != NULL);
    ASSERT_TEST(RoomsEqual(room, copy));
    RoomDestroy(room);
    RoomDestroy(copy);
    return true;
}

static bool testRoomHasBookings() {
    ASSERT_TEST(!RoomHasBookings(NULL));
    EscapeRoom room = RoomCreate(1, 8, 3, "01-23", 3);
    ASSERT_TEST(!RoomHasBookings(room));
    Booking a = BookingCreate(1, 2, 4, 4, "usern@1131", "21@comp", 1, 4);
    RoomAddBooking(room, a);
    ASSERT_TEST(RoomHasBookings(room));
    RoomDestroy(room);
    BookingDestroy(a);
    return true;
}

static bool testRoomGetId() {
    ASSERT_TEST(RoomGetId(NULL) == 0);
    EscapeRoom room = RoomCreate(1, 8, 3, "01-23", 3);
    ASSERT_TEST(RoomGetId(room) == 1);
    RoomDestroy(room);
    return true;
}

static bool testRoomGetPrice() {
    ASSERT_TEST(RoomGetPrice(NULL) == -1);
    EscapeRoom room = RoomCreate(1, 8, 3, "01-23", 3);
    ASSERT_TEST(RoomGetPrice(room) == 8);
    RoomDestroy(room);
    return true;     
}

static bool testRoomUserHasBookings() {
    ASSERT_TEST(!RoomUserHasBookings(NULL, NULL, 2, 1));
    char* str = "hi@hi";
    ASSERT_TEST(!RoomUserHasBookings(NULL, str, 2, 1));
    EscapeRoom room = RoomCreate(1, 8, 3, "01-23", 3);
    ASSERT_TEST(!RoomUserHasBookings(room, NULL, 2, 1));
    ASSERT_TEST(!RoomUserHasBookings(room, str, 2, 1));
    Booking a = BookingCreate(1, 2, 4, 4, "usern@1131", "21@comp", 1, 4);
    RoomAddBooking(room, a);
    ASSERT_TEST(!RoomUserHasBookings(room, str, 1, 2));
    Booking b = BookingCreate(1, 2, 4, 4, "hi@hi", "21@comp", 1, 4);
    RoomAddBooking(room, b);
    ASSERT_TEST(RoomUserHasBookings(room, str, 2, 1));
    RoomDestroy(room);
    BookingDestroy(a);
    BookingDestroy(b);
    return true;
}

static bool testRoomAvailable() {
    ASSERT_TEST(!RoomAvailable(NULL, 3, 2));
    EscapeRoom room = RoomCreate(1, 8, 3, "01-23", 3);
    ASSERT_TEST(!RoomAvailable(room, 2, 0));
    ASSERT_TEST(!RoomAvailable(room, 2, 24));
    ASSERT_TEST(RoomAvailable(room, 2, 18));
    Booking a = BookingCreate(1, 2, 4, 4, "usern@1131", "21@comp", 1, 4);
    RoomAddBooking(room, a);
    ASSERT_TEST(!RoomAvailable(room, 1, 2));
    ASSERT_TEST(RoomAvailable(room, 1, 4));
    RoomDestroy(room);
    BookingDestroy(a);
    return true;
}

static bool testRoomAddBooking() {
    ASSERT_TEST(RoomAddBooking(NULL, NULL) == ROOM_INVALID_PARAMETER);
    EscapeRoom room = RoomCreate(1, 8, 3, "01-23", 3);
    ASSERT_TEST(RoomAddBooking(room, NULL) == ROOM_INVALID_PARAMETER);
    Booking a = BookingCreate(1, 2, 4, 4, "usern@1131", "21@comp", 1, 4);
    ASSERT_TEST(RoomAddBooking(room, a) == ROOM_SUCCESS);
    RoomDestroy(room);
    BookingDestroy(a);
    return true;
}

static bool testRoomGetScore() {
    EscapeRoom room = RoomCreate(1, 8, 6, "15-17", 3);
    int score = RoomGetScore(room, 5, 4);
    ASSERT_TEST(score == 8);
    RoomDestroy(room);
    return true;
}

static bool testRoomGetClosestAvailable() {
    EscapeRoom room = RoomCreate(1, 8, 6, "15-17", 3);
    char* str = RoomGetClosestAvailable(room, 0);
    ASSERT_TEST(!strcmp(str, "0-15"));
    RoomDestroy(room);
    free(str);
    return true;
}

static bool testRoomCompare() {
    EscapeRoom rooma = RoomCreate(1, 8, 6, "15-17", 3);
    EscapeRoom roomb = RoomCreate(4, 8, 6, "09-17", 3);
    EscapeRoom roomc = RoomCreate(5, 8, 6, "11-16", 3);
    ASSERT_TEST(RoomCompare(roomb, rooma) > 0);
    ASSERT_TEST(RoomCompare(rooma, roomc) < 0);
    RoomDestroy(rooma);
    RoomDestroy(roomb);
    RoomDestroy(roomc);
    return true;
}


static bool testRoomGetTodayList () {
    EscapeRoom room = RoomCreate(1, 8, 6, "04-17", 3);
    Booking a = BookingCreate(1, 12, 4, 4, "usern@1131", "21@comp", 1, 4);
    Booking b = BookingCreate(1, 1, 4, 4, "usern@1131", "21@comp", 1, 5);
    RoomAddBooking(room, a);
    RoomAddBooking(room, b);
    Booking new_a = BookingCreate(0, 12, 4, 4, "usern@1131", "21@comp", 1, 4);
    Booking new_b = BookingCreate(0, 12, 4, 4, "usern@1131", "21@comp", 1, 4);
    List expected = listCreate(BookingCopy, BookingDestroy);
    listInsertLast(expected, new_a);
    listInsertLast(expected, new_b);
    List list = RoomGetTodayList(room);
    ASSERT_TEST(listGetSize(list) == 0);
    List good_list = RoomGetTodayList(room);
    ASSERT_TEST(listGetSize(good_list) == 2);
    ASSERT_TEST(BookingCompare(listGetFirst(expected), listGetFirst(good_list) 
                                                                    )== 0);
    ASSERT_TEST(BookingCompare(listGetNext(expected), listGetNext(good_list) 
                                                                    )== 0);                                                                
    BookingDestroy(a);
    BookingDestroy(b);
    BookingDestroy(new_a);
    BookingDestroy(new_b);
    RoomDestroy(room);
    listDestroy(expected);
    listDestroy(good_list);
    listDestroy(list);
    return true;
}

static bool testRoomGetDifficulty () {
    EscapeRoom room = RoomCreate(1, 8, 6, "15-17", 3);
    ASSERT_TEST (RoomGetDifficulty(room) == 3);
    RoomDestroy(room);
    return true;
}

static bool testRoomClearUserBookings () {
    EscapeRoom room = RoomCreate(123, 400, 5, "12-19", 7);
    User user = UserCreate("user@mail", 8, CHEMISTRY);
    Booking booking = BookingCreate(1, 16, 400, 5, "user@mail", "company@mail", 
                                                            CHEMISTRY, 123);
    ASSERT_TEST(RoomClearUserBookings(room, NULL)
                                                == ROOM_INVALID_PARAMETER);
    ASSERT_TEST(RoomClearUserBookings(room, "user@mail") == ROOM_SUCCESS);
    ASSERT_TEST(!RoomHasBookings(room));
    BookingDestroy(booking);
    UserDestroy(user);
    RoomDestroy(room);
    return true;
}

int main() {
    RUN_TEST(testRoomCreate);
    RUN_TEST(testRoomDestroy);
    RUN_TEST(testRoomCopy);
    RUN_TEST(testRoomHasBookings);
    RUN_TEST(testRoomGetId);
    RUN_TEST(testRoomGetPrice);
    RUN_TEST(testRoomUserHasBookings);
    RUN_TEST(testRoomAvailable);
    RUN_TEST(testRoomAddBooking);
    RUN_TEST(testRoomGetScore);
    RUN_TEST(testRoomGetClosestAvailable);
    RUN_TEST(testRoomCompare);
    RUN_TEST(testRoomGetTodayList);
    RUN_TEST(testRoomGetDifficulty);
    RUN_TEST(testRoomClearUserBookings);
    printf("Done! \n");
    return 0;
}