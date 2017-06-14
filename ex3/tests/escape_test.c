#include "test_utilities.h"
#include "../EscapeTechnion.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "../mtm_ex3.h"

static bool testSystemCreate() {
    EscapeTechnion techSystem = SystemCreate();
    SystemDestroy(techSystem);
    return true;
}

static bool testSystemDestroy() {
    SystemDestroy(NULL);
    EscapeTechnion techSystem = SystemCreate();
    SystemDestroy(techSystem);
    return true;
}

static bool testSystemAddCompany() {
    EscapeTechnion techSystem = SystemCreate();
    ASSERT_TEST(SystemAddCompany(techSystem, NULL, CIVIL_ENGINEERING) ==
                                                        MTM_INVALID_PARAMETER);
    ASSERT_TEST(SystemAddCompany(techSystem, "invalidlul", PHYSICS) == 
                                                        MTM_INVALID_PARAMETER);
    ASSERT_TEST(SystemAddCompany(techSystem, "yan@pan", MATHEMATICS) ==
                                                        MTM_SUCCESS);
    ASSERT_TEST(SystemAddCompany(techSystem, "yan@pan", CHEMISTRY) ==
                                                     MTM_EMAIL_ALREADY_EXISTS);
    SystemDestroy(techSystem);
    return true;
}

static bool testSystemRemoveCompany() {
    EscapeTechnion techSystem = SystemCreate();
    ASSERT_TEST(SystemAddCompany(techSystem, "yan@pan", MATHEMATICS) ==
                                                        MTM_SUCCESS);
    ASSERT_TEST(SystemRemoveCompany(techSystem, "not@exist") == 
                                             MTM_COMPANY_EMAIL_DOES_NOT_EXIST);
    ASSERT_TEST(SystemRemoveCompany(techSystem, "lulinvalid") == 
                                                        MTM_INVALID_PARAMETER);
    ASSERT_TEST(SystemRemoveCompany(techSystem, "yan@pan") == MTM_SUCCESS);
    SystemAddCompany(techSystem, "yan@pan", MATHEMATICS);
    SystemAddRoom(techSystem, "yan@pan", 123, 400, 5, "12-19", 7);
    SystemAddUser(techSystem, "user@mail", MATHEMATICS, 9);
    char *str1 = StrDuplicate("0-13");
    SystemAddOrder(techSystem, "user@mail", MATHEMATICS, 123, str1, 5);
    ASSERT_TEST(SystemRemoveCompany(techSystem, "yan@pan") == 
                                                       MTM_RESERVATION_EXISTS);
    free(str1);
    SystemDestroy(techSystem);
    return true;
}

static bool testSystemAddUser() {
    EscapeTechnion techSystem = SystemCreate();
    ASSERT_TEST(SystemAddUser(techSystem, "lulinvalid", MATHEMATICS, 7)
                                                    == MTM_INVALID_PARAMETER);
    ASSERT_TEST(SystemAddUser(techSystem, "yan@pan", MATHEMATICS, -2)
                                                    == MTM_INVALID_PARAMETER);
    ASSERT_TEST(SystemAddUser(techSystem, "yan@pan", MATHEMATICS, 7)
                                                    == MTM_SUCCESS);
    ASSERT_TEST(SystemAddUser(techSystem, "yan@pan", MATHEMATICS, 7)
                                                 == MTM_EMAIL_ALREADY_EXISTS);
    SystemDestroy(techSystem); 
    return true;
}

static bool testSystemAddRoom() {
    EscapeTechnion techSystem = SystemCreate();
    SystemAddCompany(techSystem, "yan@pan", MATHEMATICS);
    ASSERT_TEST(SystemAddRoom(techSystem, "yan@pan", 123, 400, 5, "12-19", 7)
                                                            == MTM_SUCCESS);
    ASSERT_TEST(SystemAddRoom(techSystem, "yan@pan", 123, 400, 5, "12-19", 7)
                                                   == MTM_ID_ALREADY_EXIST);
    ASSERT_TEST(SystemAddRoom(techSystem, "yan@pan", 222, 400, 5, "12d139", 7)
                                                   == MTM_INVALID_PARAMETER);
    ASSERT_TEST(SystemAddRoom(techSystem, "nope@pan", 123, 400, 5, "12-19", 7)
                                       == MTM_COMPANY_EMAIL_DOES_NOT_EXIST);
    SystemDestroy(techSystem);
    return true;
}

static bool testSystemRemoveRoom() {
    EscapeTechnion techSystem = SystemCreate();
    SystemAddCompany(techSystem, "yan@pan", CHEMISTRY);
    SystemAddRoom(techSystem, "yan@pan", 123, 400, 5, "12-19", 7);
    SystemAddRoom(techSystem, "yan@pan", 555, 400, 5, "12-19", 7);
    ASSERT_TEST(SystemRemoveRoom(techSystem, CHEMISTRY, 123) == MTM_SUCCESS);
    ASSERT_TEST(SystemRemoveRoom(techSystem, CHEMISTRY, 123) 
                                                   == MTM_ID_DOES_NOT_EXIST);
    ASSERT_TEST(SystemRemoveRoom(techSystem, CHEMISTRY, -5) 
                                                   == MTM_INVALID_PARAMETER);
    SystemAddUser(techSystem, "user@mail", CHEMISTRY, 9);
    char *str1 = StrDuplicate("0-13");
    SystemAddOrder(techSystem, "user@mail", CHEMISTRY, 555, str1, 5);
    ASSERT_TEST(SystemRemoveRoom(techSystem, CHEMISTRY, 555)
                                                  == MTM_RESERVATION_EXISTS);
    free(str1);
    SystemDestroy(techSystem);
    return true;
}

static bool testSystemRemoveUser() {
    EscapeTechnion techSystem = SystemCreate();
    ASSERT_TEST(SystemAddUser(techSystem, "yan@pan", MATHEMATICS, 7)
                                                    == MTM_SUCCESS);
    ASSERT_TEST(SystemRemoveUser(techSystem, "not@exist") == 
                                             MTM_CLIENT_EMAIL_DOES_NOT_EXIST);
    ASSERT_TEST(SystemRemoveUser(techSystem, "yan@pan") == MTM_SUCCESS);
    ASSERT_TEST(SystemRemoveUser(techSystem, "yan@pan")
                                          == MTM_CLIENT_EMAIL_DOES_NOT_EXIST);
    SystemDestroy(techSystem);
    return true;
}

static bool testSystemAddOrder() {
    EscapeTechnion techSystem = SystemCreate();
    SystemAddCompany(techSystem, "yan@pan", MATHEMATICS);
    SystemAddRoom(techSystem, "yan@pan", 1, 400, 5, "12-19", 7);
    SystemAddUser(techSystem, "user1@mail", MATHEMATICS, 9);
    SystemAddUser(techSystem, "user2@mail", MATHEMATICS, 9);
    char *str1 = StrDuplicate("0-13");
    ASSERT_TEST(SystemAddOrder(techSystem, "notexist@mail", MATHEMATICS,
                        1, str1, 5) == MTM_CLIENT_EMAIL_DOES_NOT_EXIST);
    ASSERT_TEST(SystemAddOrder(techSystem, "user1@mail", MATHEMATICS,
                                            1, str1, 5) == MTM_SUCCESS);
    char *str2 = StrDuplicate("0-13");
    ASSERT_TEST(SystemAddOrder(techSystem, "user1@mail", MATHEMATICS,
                                     1, str2, 5) == MTM_CLIENT_IN_ROOM);
    char *str3 = StrDuplicate("0-13");
    ASSERT_TEST(SystemAddOrder(techSystem, "user2@mail", MATHEMATICS,
                                     1, str3, 5) == MTM_ROOM_NOT_AVAILABLE);
    char *str4 = StrDuplicate("0-13");
    ASSERT_TEST(SystemAddOrder(techSystem, "user2@mail", MATHEMATICS,
                                     6, str4, 5) == MTM_ID_DOES_NOT_EXIST);
    char *other_str = StrDuplicate("2a5");
    ASSERT_TEST(SystemAddOrder(techSystem, "user2@mail", MATHEMATICS,
                                     1, other_str, 5) == MTM_INVALID_PARAMETER);
    free(str1);
    free(str2);
    free(str3);
    free(str4);
    free(other_str);
    SystemDestroy(techSystem);
    return true;
}

static bool testSystemRecommendedRoom() {
    EscapeTechnion techSystem = SystemCreate();
    SystemAddCompany(techSystem, "yan@pan", CHEMISTRY);
    SystemAddUser(techSystem, "user1@mail", MATHEMATICS, 3);
    ASSERT_TEST(SystemRecommendedRoom(techSystem, "user1@mail", 3) 
                                                   == MTM_NO_ROOMS_AVAILABLE);
    SystemAddRoom(techSystem, "yan@pan", 1, 8, 3, "01-17", 3);
    SystemAddRoom(techSystem, "yan@pan", 2, 8, 5, "01-18", 5);
    SystemAddRoom(techSystem, "yan@pan", 3, 8, 4, "01-18", 4);
    SystemAddRoom(techSystem, "yan@pan", 4, 400, 5, "12-19", 7);
    ASSERT_TEST(SystemRecommendedRoom(techSystem, "invalidlul", 3) 
                                                    == MTM_INVALID_PARAMETER);
    ASSERT_TEST(SystemRecommendedRoom(techSystem, "user1@mail", 3) 
                                                              == MTM_SUCCESS);

    ASSERT_TEST(SystemRecommendedRoom(techSystem, "not@exist", 3) 
                                          == MTM_CLIENT_EMAIL_DOES_NOT_EXIST);
    SystemDestroy(techSystem);
    return true;
}

static bool testSystemReportDay() {
    EscapeTechnion techSystem = SystemCreate(); 
    SystemAddCompany(techSystem, "chem@pan", CHEMISTRY);
    SystemAddCompany(techSystem, "math@pan", MATHEMATICS);
    SystemAddCompany(techSystem, "phys@pan", PHYSICS);
    SystemAddCompany(techSystem, "bio@pan", BIOLOGY);
    SystemAddRoom(techSystem, "chem@pan", 1, 300, 3, "01-17", 3);
    SystemAddRoom(techSystem, "math@pan", 2, 200, 3, "01-17", 3);
    SystemAddRoom(techSystem, "phys@pan", 3, 100, 3, "01-17", 3);
    SystemAddRoom(techSystem, "bio@pan", 4, 200, 3, "01-17", 3);
    SystemAddUser(techSystem, "user1@mail", MEDICINE, 3);
    SystemAddUser(techSystem, "user2@mail", MEDICINE, 3);
    SystemAddUser(techSystem, "user3@mail", MEDICINE, 3);
    SystemAddUser(techSystem, "user4@mail", MEDICINE, 3);
    char *str1 = StrDuplicate("0-13");
    SystemAddOrder(techSystem, "user1@mail", CHEMISTRY, 1, str1, 5);
    free(str1);
    str1 = StrDuplicate("0-13");
    SystemAddOrder(techSystem, "user2@mail", MATHEMATICS, 2, str1, 5);
    free(str1);
    str1 = StrDuplicate("0-13");
    SystemAddOrder(techSystem, "user3@mail", PHYSICS, 3, str1, 5);
    free(str1);
    str1 = StrDuplicate("0-13");
    SystemAddOrder(techSystem, "user4@mail", BIOLOGY, 4, str1, 5);
    SystemReportDay(techSystem, stdout);
    free(str1);
    SystemDestroy(techSystem);
    return true;
}

static bool testSystemReportBest() {
    EscapeTechnion techSystem = SystemCreate(); 
    SystemReportBest(techSystem, stdout);
    SystemAddCompany(techSystem, "chem@pan", CHEMISTRY);
    SystemAddCompany(techSystem, "math@pan", MATHEMATICS);
    SystemAddCompany(techSystem, "phys@pan", PHYSICS);
    SystemAddCompany(techSystem, "bio@pan", BIOLOGY);
    SystemAddRoom(techSystem, "chem@pan", 1, 300, 3, "01-17", 3);
    SystemAddRoom(techSystem, "math@pan", 2, 200, 3, "01-17", 3);
    SystemAddRoom(techSystem, "phys@pan", 3, 100, 3, "01-17", 3);
    SystemAddRoom(techSystem, "bio@pan", 4, 200, 3, "01-17", 3);
    SystemAddUser(techSystem, "user1@mail", MEDICINE, 3);
    SystemAddUser(techSystem, "user2@mail", MEDICINE, 3);
    SystemAddUser(techSystem, "user3@mail", MEDICINE, 3);
    SystemAddUser(techSystem, "user4@mail", MEDICINE, 3);
    char *str1 = StrDuplicate("0-13");
    SystemAddOrder(techSystem, "user1@mail", CHEMISTRY, 1, str1, 5);
    free(str1);
    str1 = StrDuplicate("0-13");
    SystemAddOrder(techSystem, "user2@mail", MATHEMATICS, 2, str1, 5);
    free(str1);
    str1 = StrDuplicate("0-13");
    SystemAddOrder(techSystem, "user3@mail", PHYSICS, 3, str1, 5);
    free(str1);
    str1 = StrDuplicate("0-13");
    SystemAddOrder(techSystem, "user4@mail", BIOLOGY, 4, str1, 5);
    SystemReportDay(techSystem, stdout);
    SystemReportBest(techSystem, NULL);
    SystemReportBest(techSystem, stdout);
    free(str1);
    SystemDestroy(techSystem);
    return true;
}

int main() {
    RUN_TEST(testSystemCreate);
    RUN_TEST(testSystemDestroy);
    RUN_TEST(testSystemAddCompany);
    RUN_TEST(testSystemRemoveCompany);
    RUN_TEST(testSystemAddUser);
    RUN_TEST(testSystemRemoveUser);
    RUN_TEST(testSystemAddRoom);
    RUN_TEST(testSystemRemoveRoom);
    RUN_TEST(testSystemAddOrder);
    RUN_TEST(testSystemRecommendedRoom);
    RUN_TEST(testSystemReportDay);
    RUN_TEST(testSystemReportBest);
    printf("Done! \n");
    return 0;
}