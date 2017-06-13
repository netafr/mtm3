#include "test_utilities.h"
#include "../../Faculty.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "../../mtm_ex3.h"

static bool testFacultyCreate() {
    Faculty faculty = FacultyCreate(CIVIL_ENGINEERING);
    ASSERT_TEST(FacultyGetName(faculty) == CIVIL_ENGINEERING);
    FacultyDestroy(faculty);
    return true;
}

static bool testFacultyDestroy() {
    FacultyDestroy(NULL);
    Faculty faculty = FacultyCreate(CIVIL_ENGINEERING);
    FacultyDestroy(faculty);
    return true;
}

static bool testFacultyCopy() {
    ASSERT_TEST(FacultyCopy(NULL) == NULL);
    Faculty faculty = FacultyCreate(1);
    FacultyAddProfit(faculty, 200);
    Faculty copy = FacultyCopy(faculty);
    ASSERT_TEST(FacultyCompare(faculty, copy) == 0);
    ASSERT_TEST(FacultyProfitCompare(faculty, copy)== 0);
    FacultyDestroy(faculty);
    FacultyDestroy(copy);
    return true;
}
static bool testFacultyCompare() {
    Faculty facultya = FacultyCreate(PHYSICS);
    Faculty facultyb = FacultyCreate(MATHEMATICS);
    ASSERT_TEST(FacultyCompare(facultya, facultyb) > 0);
    FacultyDestroy(facultya);
    FacultyDestroy(facultyb);
    return true;
}

static bool testFacultyProfitCompare() {
    Faculty facultya = FacultyCreate(PHYSICS);
    Faculty facultyb = FacultyCreate(MATHEMATICS);
    FacultyAddProfit(facultya, 100);
    FacultyAddProfit(facultyb, 200);
    ASSERT_TEST(FacultyProfitCompare(facultya, facultyb) < 0);
    FacultyDestroy(facultya);
    FacultyDestroy(facultyb);
    return true;
}

static bool testFacultyGetName() {
    ASSERT_TEST(FacultyGetName(NULL) == UNKNOWN);
    Faculty faculty = FacultyCreate(PHYSICS);
    ASSERT_TEST(FacultyGetName(faculty) == PHYSICS);
    FacultyDestroy(faculty);
    return true;
}

static bool testFacultyAddProfit() {
    FacultyAddProfit(NULL, 10);
    Faculty faculty = FacultyCreate(1);
    ASSERT_TEST(FacultyGetProfit(faculty) == 0);
    FacultyAddProfit(faculty, 200);
    ASSERT_TEST(FacultyGetProfit(faculty) == 200);
    FacultyDestroy(faculty);
    return true;
}

static bool testFacultyGetProfit() {
    ASSERT_TEST(FacultyGetProfit(NULL) == -1);
    Faculty faculty = FacultyCreate(1);
    ASSERT_TEST(FacultyGetProfit(faculty) == 0);
    FacultyAddProfit(faculty, 200);
    ASSERT_TEST(FacultyGetProfit(faculty) == 200);
    FacultyDestroy(faculty);
    return true;
}

static bool testFacultyCompanyExists() {
    ASSERT_TEST(!FacultyCompanyExists(NULL, NULL));
    Faculty faculty = FacultyCreate(1);
    EscapeCompany company = CompanyCreate("hi@test");
    ASSERT_TEST(!FacultyCompanyExists(faculty, NULL));
    ASSERT_TEST(!FacultyCompanyExists(NULL, CompanyGetEmail(company)));
    ASSERT_TEST(!FacultyCompanyExists(faculty, CompanyGetEmail(company)));
    ASSERT_TEST(FacultyInsertCompany(faculty, company) == FACULTY_SUCCESS);
    ASSERT_TEST(FacultyCompanyExists(faculty, CompanyGetEmail(company)));
    CompanyDestroy(company);
    FacultyDestroy(faculty);
    return true;
}

static bool testFacultyInsertCompany() {
    ASSERT_TEST(FacultyInsertCompany(NULL, NULL) == FACULTY_INVALID_PARAMETER);
    Faculty faculty = FacultyCreate(1);
    EscapeCompany company = CompanyCreate("hi@test");
    ASSERT_TEST(FacultyInsertCompany(faculty, NULL) ==
                                                    FACULTY_INVALID_PARAMETER);
    ASSERT_TEST(FacultyInsertCompany(NULL, company) == 
                                                    FACULTY_INVALID_PARAMETER);
    ASSERT_TEST(FacultyInsertCompany(faculty, company) == FACULTY_SUCCESS);
    ASSERT_TEST(FacultyInsertCompany(faculty, company) == 
                                                    FACULTY_COMPANY_EXISTS);
    CompanyDestroy(company);
    FacultyDestroy(faculty);
    return true;
}

static bool testFacultyGetCompany() {
    ASSERT_TEST(FacultyGetCompany(NULL, NULL) == NULL);
    Faculty faculty = FacultyCreate(1);
    EscapeCompany company = CompanyCreate("hi@test");
    ASSERT_TEST(FacultyGetCompany(faculty, NULL) == NULL);
    ASSERT_TEST(FacultyGetCompany(NULL, CompanyGetEmail(company)) == NULL);
    ASSERT_TEST(FacultyGetCompany(faculty, CompanyGetEmail(company)) == NULL);
    ASSERT_TEST(FacultyInsertCompany(faculty, company) == FACULTY_SUCCESS);
    EscapeCompany test = FacultyGetCompany(faculty, CompanyGetEmail(company));
    ASSERT_TEST(test != NULL);
    CompanyDestroy(company);
    FacultyDestroy(faculty);
    return true;
}

static bool testFacultyRemoveCompany() {
    Faculty faculty = FacultyCreate(1);
    EscapeCompany company = CompanyCreate("hi@test");
    ASSERT_TEST(FacultyRemoveCompany(NULL, NULL) == FACULTY_INVALID_PARAMETER);
    ASSERT_TEST(FacultyRemoveCompany(faculty, NULL) == 
                                                     FACULTY_INVALID_PARAMETER);
    ASSERT_TEST(FacultyRemoveCompany(NULL, company) == 
                                                    FACULTY_INVALID_PARAMETER);
    ASSERT_TEST(FacultyRemoveCompany(faculty, company) == 
                                                    FACULTY_INVALID_PARAMETER);
    ASSERT_TEST(FacultyInsertCompany(faculty, company) == FACULTY_SUCCESS);
    ASSERT_TEST(FacultyRemoveCompany(faculty, company) == FACULTY_SUCCESS);
    CompanyDestroy(company);
    FacultyDestroy(faculty);
    return true;
}

static bool testFacultyGetRoom() {
    Faculty faculty = FacultyCreate(1);
    EscapeCompany company = CompanyCreate("hi@test");
    EscapeRoom room = RoomCreate(1, 8, 3, "01-23", 3);
    CompanyInsertRoom(company, room);
    EscapeCompany wanted;
    ASSERT_TEST(FacultyGetRoom(NULL, -1, NULL) == NULL);
    ASSERT_TEST(FacultyGetRoom(NULL, 1, &wanted) == NULL);
    ASSERT_TEST(FacultyGetRoom(faculty, -1, &wanted) == NULL);
    ASSERT_TEST(FacultyGetRoom(faculty, 1, &wanted) == NULL);
    FacultyInsertCompany(faculty, company);
    ASSERT_TEST(FacultyGetRoom(faculty, 1, &wanted) != NULL);
    RoomDestroy(room);
    CompanyDestroy(company);
    FacultyDestroy(faculty);
    return true;
}

static bool testFacultyUserHasBookings() {
    ASSERT_TEST(!FacultyUserHasBookings(NULL, NULL, 1, 2));
    Faculty faculty = FacultyCreate(1);
    EscapeCompany company1 = CompanyCreate("hi@test");
    EscapeCompany company2 = CompanyCreate("other@test");
    EscapeRoom rooma = RoomCreate(1, 8, 6, "01-17", 3);
    EscapeRoom roomb = RoomCreate(2, 8, 6, "01-17", 3);
    EscapeRoom roomc = RoomCreate(3, 8, 6, "01-16", 3);
    Booking a = BookingCreate(2, 1, 4, 4, "usern@1131", "hi@test", 1, 4);
    Booking b = BookingCreate(1, 2, 4, 4, "usern@2131", "hi@test", 2, 5);
    Booking c = BookingCreate(2, 1, 4, 4, "usern@3131", "other@test", 3, 6);
    RoomAddBooking(rooma, a);
    RoomAddBooking(roomb, b);
    RoomAddBooking(roomc, c);
    CompanyInsertRoom(company1, rooma);
    CompanyInsertRoom(company2, roomc);
    CompanyInsertRoom(company1, roomb);
    ASSERT_TEST(!FacultyUserHasBookings(faculty, NULL, 1, 2));
    ASSERT_TEST(!FacultyUserHasBookings(NULL, "usern@1131", 1, 2));
    ASSERT_TEST(!FacultyUserHasBookings(faculty, "usern@1131", 1, 2));
    FacultyInsertCompany(faculty, company1);
    FacultyInsertCompany(faculty, company2);
    ASSERT_TEST(!FacultyUserHasBookings(faculty, "usern@2131", 1, 2));
    ASSERT_TEST(FacultyUserHasBookings(faculty, "usern@1131", 1, 2));
    BookingDestroy(a);
    BookingDestroy(b);
    BookingDestroy(c);
    RoomDestroy(rooma);
    RoomDestroy(roomb);
    RoomDestroy(roomc);
    CompanyDestroy(company1);
    CompanyDestroy(company2);
    FacultyDestroy(faculty);
    return true;
}

static bool testFacultyHasRooms() {
    Faculty faculty = FacultyCreate(1);
    EscapeCompany company1 = CompanyCreate("hi@test");
    EscapeCompany company2 = CompanyCreate("other@test");
    EscapeRoom rooma = RoomCreate(1, 8, 6, "01-17", 3);
    EscapeRoom roomb = RoomCreate(2, 8, 6, "01-17", 3);
    CompanyInsertRoom(company1, rooma);
    CompanyInsertRoom(company1, roomb);
    ASSERT_TEST(!FacultyHasRooms(NULL));
    ASSERT_TEST(!FacultyHasRooms(faculty));
    FacultyInsertCompany(faculty, company2);
    ASSERT_TEST(!FacultyHasRooms(faculty));
    FacultyInsertCompany(faculty, company1);
    ASSERT_TEST(FacultyHasRooms(faculty));
    RoomDestroy(rooma);
    RoomDestroy(roomb);
    CompanyDestroy(company1);
    CompanyDestroy(company2);
    FacultyDestroy(faculty);
    return true;
}

static bool testFacultyGetRecommendedRoom() {
    int score;
    Faculty faculty = FacultyCreate(1);
    EscapeCompany company1 = CompanyCreate("hi@test");
    EscapeCompany company2 = CompanyCreate("other@test");
    EscapeCompany company3 = CompanyCreate("empty@test");
    EscapeRoom room1 = RoomCreate(1, 8, 3, "01-17", 3);
    EscapeRoom room2 = RoomCreate(2, 8, 5, "01-18", 5);
    EscapeRoom room3 = RoomCreate(3, 8, 3, "01-18", 3);
    EscapeRoom room4 = RoomCreate(4, 8, 2, "01-14", 2);
    CompanyInsertRoom(company1, room1);
    CompanyInsertRoom(company1, room2);
    CompanyInsertRoom(company2, room3);
    ASSERT_TEST(FacultyGetRecommenedRoom(NULL, 2, 2, &score) == NULL);
    ASSERT_TEST(FacultyGetRecommenedRoom(faculty, 2, 2, NULL) == NULL);
    ASSERT_TEST(FacultyGetRecommenedRoom(faculty, 2, 2, &score) == NULL);
    FacultyInsertCompany(faculty, company1);
    FacultyInsertCompany(faculty, company2);
    FacultyInsertCompany(faculty, company3);
    ASSERT_TEST(FacultyGetRecommenedRoom(faculty, 2, 2, &score) != NULL);
    ASSERT_TEST(score == 2);
    EscapeCompany company4 = CompanyCreate("score@test");
    CompanyInsertRoom(company4, room4);
    FacultyInsertCompany(faculty, company4);
    ASSERT_TEST(FacultyGetRecommenedRoom(faculty, 2, 2, &score) != NULL);
    ASSERT_TEST(score == 0);
    RoomDestroy(room1);
    RoomDestroy(room2);
    RoomDestroy(room3);
    RoomDestroy(room4);
    CompanyDestroy(company1);
    CompanyDestroy(company2);
    CompanyDestroy(company3);
    CompanyDestroy(company4);
    FacultyDestroy(faculty);
    return true;
}

static bool testFacultyGetTodayList() {
    ASSERT_TEST(FacultyGetTodayList(NULL) == NULL);
    Faculty faculty = FacultyCreate(1);
    List list = FacultyGetTodayList(faculty);
    EscapeCompany company1 = CompanyCreate("1@test");
    EscapeCompany company2 = CompanyCreate("2@test");
    EscapeCompany company3 = CompanyCreate("3@test");
    EscapeRoom room1 = RoomCreate(1, 8, 3, "01-17", 3);
    EscapeRoom room2 = RoomCreate(2, 8, 3, "01-18", 3);
    Booking a = BookingCreate(1, 2, 4, 4, "usern@1131", "21@comp", 1, 1);
    Booking b = BookingCreate(2, 3, 4, 4, "usern@2131", "31@comp", 2, 1);
    Booking c = BookingCreate(2, 2, 4, 4, "usern@3131", "41@comp", 3, 2);
    RoomAddBooking(room1, a);
    RoomAddBooking(room1, b);
    RoomAddBooking(room2, c);
    CompanyInsertRoom(company1, room1);
    CompanyInsertRoom(company2, room2);
    FacultyInsertCompany(faculty, company1);
    FacultyInsertCompany(faculty, company2);
    FacultyInsertCompany(faculty, company3);
    List day1 = FacultyGetTodayList(faculty);
    ASSERT_TEST(listGetSize(day1) == 0);
    List day2 = FacultyGetTodayList(faculty);
    ASSERT_TEST(listGetSize(day2) == 1);
    List day3 = FacultyGetTodayList(faculty);
    ASSERT_TEST(listGetSize(day3) == 2);
    List day4 = FacultyGetTodayList(faculty);
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
    CompanyDestroy(company1);
    CompanyDestroy(company2);
    CompanyDestroy(company3);
    FacultyDestroy(faculty);
    return true;
}

static bool testFacultyRoomExists() {
    Faculty faculty = FacultyCreate(1);
    EscapeCompany company1 = CompanyCreate("hi@test");
    EscapeCompany company2 = CompanyCreate("other@test");
    EscapeCompany company3 = CompanyCreate("empty@test");
    EscapeRoom room1 = RoomCreate(1, 8, 3, "01-17", 3);
    EscapeRoom room2 = RoomCreate(2, 8, 5, "01-18", 5);
    EscapeRoom room3 = RoomCreate(3, 8, 3, "01-18", 3);
    EscapeRoom room4 = RoomCreate(4, 8, 2, "01-14", 2);
    CompanyInsertRoom(company1, room1);
    CompanyInsertRoom(company1, room2);
    CompanyInsertRoom(company2, room3);
    FacultyInsertCompany(faculty, company1);
    FacultyInsertCompany(faculty, company2);
    FacultyInsertCompany(faculty, company3);
    ASSERT_TEST(FacultyRoomExists(faculty, 1));
    ASSERT_TEST(!FacultyRoomExists(faculty, 5));
    FacultyRemoveCompany(faculty, company3);
    ASSERT_TEST(FacultyRoomExists(faculty, 1));
    FacultyRemoveCompany(faculty, company1);
    ASSERT_TEST(!FacultyRoomExists(faculty, 1));
    EscapeCompany company4 = CompanyCreate("lst@test");
    EscapeRoom room5 = RoomCreate(5, 8, 2, "02-14", 2);
    CompanyInsertRoom(company4, room5);
    FacultyInsertCompany(faculty, company4);
    ASSERT_TEST(FacultyRoomExists(faculty, 5));
    RoomDestroy(room1);
    RoomDestroy(room2);
    RoomDestroy(room3);
    RoomDestroy(room4);
    RoomDestroy(room5);
    CompanyDestroy(company1);
    CompanyDestroy(company2);
    CompanyDestroy(company3);
    CompanyDestroy(company4);
    FacultyDestroy(faculty);
    return true;
}

int main () {
    RUN_TEST(testFacultyCreate);
    RUN_TEST(testFacultyDestroy);
    RUN_TEST(testFacultyCopy);
    RUN_TEST(testFacultyCompare);
    RUN_TEST(testFacultyProfitCompare);
    RUN_TEST(testFacultyGetName);
    RUN_TEST(testFacultyAddProfit);
    RUN_TEST(testFacultyGetProfit);
    RUN_TEST(testFacultyCompanyExists);
    RUN_TEST(testFacultyInsertCompany);
    RUN_TEST(testFacultyGetCompany);
    RUN_TEST(testFacultyRemoveCompany);
    RUN_TEST(testFacultyGetRoom);
    RUN_TEST(testFacultyUserHasBookings);
    RUN_TEST(testFacultyHasRooms);
    RUN_TEST(testFacultyGetRecommendedRoom); 
    RUN_TEST(testFacultyGetTodayList);
    //ADD testFaculty
    RUN_TEST(testFacultyRoomExists);
    printf("Done! \n");
}