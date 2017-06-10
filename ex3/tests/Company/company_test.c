#include "test_utilities.h"
#include "../../EscapyCompany.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "../../mtm_ex3.h"

static bool testCompanyCreate() {
    ASSERT_TEST(CompanyCreate(NULL) == NULL);
    ASSERT_TEST(CompanyCreate("QQa") == NULL);
    EscapeCompany company = CompanyCreate("user@yanyan");
    ASSERT_TEST(company != NULL);
    CompanyDestroy(company);
    return true;
}
static bool testCompanyDestroy() {
    CompanyDestroy(NULL);
    EscapeCompany company = CompanyCreate("user@yanyan");
    CompanyDestroy(company);
    return true;
}

static bool testCompanyCopy() {
    ASSERT_TEST(CompanyCopy(NULL) == NULL);
    EscapeCompany company = CompanyCreate("user@yanyan");
    EscapeCompany copy = CompanyCopy(company);
    ASSERT_TEST(strcmp(CompanyGetEmail(company), CompanyGetEmail(copy)) == 0);
    CompanyDestroy(company);
    CompanyDestroy(copy);
    return true;
}

static bool testCompanyCompare() {
    EscapeCompany company = CompanyCreate("user@yanyan");
    EscapeCompany copy = CompanyCopy(company);
    EscapeCompany another = CompanyCreate("user2@yanyan");
    EscapeCompany last = CompanyCreate("user@notyan");
    ASSERT_TEST(CompanyCompare(company, copy) == 0);
    ASSERT_TEST(CompanyCompare(company, another) > 0);
    ASSERT_TEST(CompanyCompare(company, last) > 0);
    CompanyDestroy(company);
    CompanyDestroy(copy);
    CompanyDestroy(another);
    CompanyDestroy(last);
    return true;
}

static bool testCompanyGetEmail() {
    ASSERT_TEST(CompanyGetEmail(NULL) == NULL);
    EscapeCompany company = CompanyCreate("user@yanyan");
    ASSERT_TEST(strcmp(CompanyGetEmail(company),"user@yanyan") == 0);
    CompanyDestroy(company);
    return true;
}

static bool testCompanyInsertRoom() {
    EscapeCompany company = CompanyCreate("user@yanyan");
    EscapeRoom room = RoomCreate(1, 8, 3, "15-17", 3);
    ASSERT_TEST(CompanyInsertRoom(NULL, NULL) == COMPANY_INVALID_PARAMETER);
    ASSERT_TEST(CompanyInsertRoom(company, NULL) == COMPANY_INVALID_PARAMETER);
    ASSERT_TEST(CompanyInsertRoom(NULL, room) == COMPANY_INVALID_PARAMETER);
    ASSERT_TEST(CompanyInsertRoom(company, room) == COMPANY_SUCCESS);
    EscapeRoom wanted_room = CompanyGetRoom(company, 1);
    ASSERT_TEST(RoomGetId(wanted_room) == RoomGetId(room));
    RoomDestroy(room);
    CompanyDestroy(company);
    return true;
}

static bool testCompanyRoomExists() {
    EscapeCompany company = CompanyCreate("user@yanyan");
    EscapeRoom room = RoomCreate(1, 8, 3, "15-17", 3);
    ASSERT_TEST(!CompanyRoomExists(NULL, -1));
    ASSERT_TEST(!CompanyRoomExists(NULL, 1));
    ASSERT_TEST(!CompanyRoomExists(company, -1));
    ASSERT_TEST(!CompanyRoomExists(company, 1));
    CompanyInsertRoom(company, room);
    ASSERT_TEST(!CompanyRoomExists(company, 2));
    ASSERT_TEST(CompanyRoomExists(company, 1));
    RoomDestroy(room);
    CompanyDestroy(company);
    return true;
}

static bool testCompanyHasBookings() {
    ASSERT_TEST(!CompanyHasBookings(NULL));
    EscapeCompany company = CompanyCreate("user@yanyan");
    EscapeRoom room = RoomCreate(1, 8, 3, "15-17", 3);
    ASSERT_TEST(!CompanyHasBookings(company));
    Booking a = BookingCreate(1, 2, 4, 4, "usern@1131", "21@comp", 1, 4);
    RoomAddBooking(room, a);
    CompanyInsertRoom(company, room);
    ASSERT_TEST(CompanyHasBookings(company));
    BookingDestroy(a);
    RoomDestroy(room);
    CompanyDestroy(company);
    return true;
}

static bool testCompanyRemoveRoom() {
    EscapeCompany company = CompanyCreate("user@yanyan");
    EscapeRoom room = RoomCreate(1, 8, 3, "15-17", 3);
    ASSERT_TEST(CompanyRemoveRoom(NULL, NULL) == COMPANY_INVALID_PARAMETER);
    ASSERT_TEST(CompanyRemoveRoom(company, NULL) == COMPANY_INVALID_PARAMETER);
    ASSERT_TEST(CompanyRemoveRoom(NULL, room) == COMPANY_INVALID_PARAMETER);
    ASSERT_TEST(CompanyRemoveRoom(company, room) == COMPANY_SUCCESS);
    CompanyInsertRoom(company, room);
    ASSERT_TEST(CompanyRoomExists(company, 1));
    ASSERT_TEST(CompanyRemoveRoom(company, room) == COMPANY_SUCCESS);
    ASSERT_TEST(!CompanyRoomExists(company, 1));
    RoomDestroy(room);
    CompanyDestroy(company);
    return true;
}

static bool testCompanyGetRoom() {
    EscapeCompany company = CompanyCreate("user@yanyan");
    EscapeRoom room = RoomCreate(1, 8, 3, "15-17", 3);
    ASSERT_TEST(CompanyGetRoom(NULL, -1) == NULL);
    ASSERT_TEST(CompanyGetRoom(NULL, 1) == NULL);
    ASSERT_TEST(CompanyGetRoom(company, -1) == NULL);
    ASSERT_TEST(CompanyGetRoom(company, 1) == NULL);
    CompanyInsertRoom(company, room);
    ASSERT_TEST(RoomGetId(CompanyGetRoom(company, 1)) == RoomGetId(room));
    RoomDestroy(room);
    CompanyDestroy(company);
    return true;
}

static bool testCompanyUserHasBookings() {
    EscapeCompany company = CompanyCreate("user@yanyan");
    EscapeRoom room = RoomCreate(1, 8, 3, "15-17", 3);
    Booking a = BookingCreate(1, 2, 4, 4, "usern@1131", "21@comp", 1, 4);
    ASSERT_TEST(!CompanyUserHasBookings(NULL, NULL, 2, 1));
    ASSERT_TEST(!CompanyUserHasBookings(company, NULL, 2, 1));
    ASSERT_TEST(!CompanyUserHasBookings(NULL, "usern@1131", 2, 1));
    ASSERT_TEST(!CompanyUserHasBookings(company, "usern@1131", 2, 1));
    RoomAddBooking(room, a);
    CompanyInsertRoom(company, room);
    ASSERT_TEST(!CompanyUserHasBookings(company, "usern@1131", 2, 2));
    ASSERT_TEST(CompanyUserHasBookings(company, "usern@1131", 2, 1));
    BookingDestroy(a);
    RoomDestroy(room);
    CompanyDestroy(company);
    return true;
}

static bool testCompanyHasRooms() {
    EscapeCompany company = CompanyCreate("user@yanyan");
    EscapeRoom room = RoomCreate(1, 8, 3, "15-17", 3);
    ASSERT_TEST(!CompanyHasRooms(NULL));
    ASSERT_TEST(!CompanyHasRooms(company));
    CompanyInsertRoom(company, room);
    ASSERT_TEST(CompanyHasRooms(company));
    RoomDestroy(room);
    CompanyDestroy(company);
    return true;
}

static bool testCompanyGetRecommendedRoom() {
    EscapeCompany company = CompanyCreate("user@yanyan");
    int id, score;
    ASSERT_TEST(CompanyGetRecommendedRoom(company, 1, 1, NULL, &score) == NULL);
    ASSERT_TEST(CompanyGetRecommendedRoom(NULL, 1, 1, &id, &score) == NULL);
    ASSERT_TEST(CompanyGetRecommendedRoom(company, 1, 1, &id, NULL) == NULL);
    ASSERT_TEST(CompanyGetRecommendedRoom(company, 1, 1, &id, &score) == NULL);
    EscapeRoom room1 = RoomCreate(1, 8, 3, "1-17", 3);
    EscapeRoom room2 = RoomCreate(2, 8, 5, "1-18", 5);
    EscapeRoom room3 = RoomCreate(3, 8, 3, "1-18", 3);
    CompanyInsertRoom(company, room1);
    CompanyInsertRoom(company, room2);
    CompanyInsertRoom(company, room3);
    ASSERT_TEST(CompanyGetRecommendedRoom(company, 2, 2, &id, &score) != NULL);
    ASSERT_TEST(id == 1 && score == 2);
    EscapeRoom room4 = RoomCreate(4, 8, 2, "1-14", 2);
    CompanyInsertRoom(company, room4);
    ASSERT_TEST(CompanyGetRecommendedRoom(company, 2, 2, &id, &score) != NULL);
    ASSERT_TEST(id == 4 && score == 0);
    RoomDestroy(room1);
    RoomDestroy(room2);
    RoomDestroy(room3);
    RoomDestroy(room4);
    CompanyDestroy(company);
    return true;
}

static bool testCompanyGetTodayList() {
    ASSERT_TEST(CompanyGetTodayList(NULL) == NULL);
    EscapeCompany company = CompanyCreate("user@yanyan");
    List list = CompanyGetTodayList(company);
    ASSERT_TEST(listGetSize(list) == 0);
    EscapeRoom room1 = RoomCreate(1, 8, 3, "1-17", 3);
    EscapeRoom room2 = RoomCreate(2, 8, 3, "1-18", 3);
    Booking a = BookingCreate(1, 2, 4, 4, "usern@1131", "21@comp", 1, 1);
    Booking b = BookingCreate(2, 3, 4, 4, "usern@2131", "31@comp", 2, 1);
    Booking c = BookingCreate(2, 2, 4, 4, "usern@3131", "41@comp", 3, 2);
    RoomAddBooking(room1, a);
    RoomAddBooking(room1, b);
    RoomAddBooking(room2, c);
    CompanyInsertRoom(company, room1);
    CompanyInsertRoom(company, room2);
    List day1 = CompanyGetTodayList(company);
    ASSERT_TEST(listGetSize(day1) == 0);
    List day2 = CompanyGetTodayList(company);
    ASSERT_TEST(listGetSize(day2) == 1);
    List day3 = CompanyGetTodayList(company);
    ASSERT_TEST(listGetSize(day3) == 2);
    List day4 = CompanyGetTodayList(company);
    ASSERT_TEST(listGetSize(day4) == 0);
    listDestroy(day1);
    listDestroy(day2);
    listDestroy(day3);
    listDestroy(day4);
    listDestroy(list);
    BookingDestroy(a);
    BookingDestroy(b);
    BookingDestroy(c);
    RoomDestroy(room1);
    RoomDestroy(room2);
    CompanyDestroy(company);
    return true;
}

int main() {
    RUN_TEST(testCompanyCreate);
    RUN_TEST(testCompanyDestroy);
    RUN_TEST(testCompanyCopy);
    RUN_TEST(testCompanyCompare);
    RUN_TEST(testCompanyGetEmail);
    RUN_TEST(testCompanyInsertRoom);
    RUN_TEST(testCompanyRoomExists);
    RUN_TEST(testCompanyHasBookings);
    RUN_TEST(testCompanyRemoveRoom);
    RUN_TEST(testCompanyGetRoom);
    RUN_TEST(testCompanyUserHasBookings);
    RUN_TEST(testCompanyHasRooms);
    RUN_TEST(testCompanyGetRecommendedRoom);
    RUN_TEST(testCompanyGetTodayList);
    printf("Done! \n");
}