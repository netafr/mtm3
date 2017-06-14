#include "../User.h" 
#include "test_utilities.h"


static bool CreateTest() {
    User user = UserCreate("notemail", 3, COMPUTER_SCIENCE);
    ASSERT_TEST(user == NULL);
    user = UserCreate("not@email@too", 3, ARCHITECTURE);
    ASSERT_TEST(user == NULL);
    user = UserCreate("email@email.com", 3, CHEMISTRY);
    ASSERT_TEST(user != NULL);
    UserDestroy(user);
    return true;
}

static bool DestroyTest() {
    UserDestroy(NULL);
    User user = UserCreate("email@email.com", 2, AEROSPACE_ENGINEERING);
    UserDestroy(user);
    return true;
}

static bool CopyTest() {
    ASSERT_TEST(UserCopy(NULL) == NULL);
    User user = UserCreate("email@email.com", 2, AEROSPACE_ENGINEERING);
    User copy = UserCopy(user);
    ASSERT_TEST(strcmp(UserGetEmail(user), UserGetEmail(copy)) == 0);
    ASSERT_TEST(UserGetFaculty(user) == UserGetFaculty(copy));
    ASSERT_TEST(UserGetLevel(user) == UserGetLevel(copy));
    UserDestroy(user);
    UserDestroy(copy);
    return true;
}

static bool CompareTest() {
    User user = UserCreate("email@email.com", 2, AEROSPACE_ENGINEERING);
    User copy = UserCopy(user);
    User other = UserCreate("aemail@email.com", 4, CHEMICAL_ENGINEERING);
    ASSERT_TEST(UserCompare(user, copy) == 0);
    ASSERT_TEST(UserCompare(user, other) > 0);
    ASSERT_TEST(UserCompare(other, user) < 0);
    ASSERT_TEST(UserCompare(copy, copy) == 0);
    UserDestroy(user);
    UserDestroy(copy);
    UserDestroy(other);
    return true;
}

static bool GetLevelTest() {
    User user = UserCreate("email@email.com", 2, AEROSPACE_ENGINEERING);
    User copy = UserCopy(user);
    User other = UserCreate("aemail@email.com", 4, CHEMICAL_ENGINEERING);
    ASSERT_TEST(UserGetLevel(user) == 2);
    ASSERT_TEST(UserGetLevel(other) == 4 );
    ASSERT_TEST(UserGetLevel(user) == UserGetLevel(copy));
    UserDestroy(user);
    UserDestroy(copy);
    UserDestroy(other);
    return true;
}

static bool GetEmailTest() {
    User user = UserCreate("email@email.com", 2, AEROSPACE_ENGINEERING);
    User copy = UserCopy(user);
    User other = UserCreate("aemail@email.com", 4, CHEMICAL_ENGINEERING);
    ASSERT_TEST(strcmp(UserGetEmail(user),"email@email.com") == 0);
    ASSERT_TEST(strcmp(UserGetEmail(other), "aemail@email.com") == 0);
    ASSERT_TEST(strcmp(UserGetEmail(copy), UserGetEmail(user)) == 0);
    UserDestroy(user);
    UserDestroy(copy);
    UserDestroy(other);
    return true;
}

static bool GetFacultyTest() {
    User user = UserCreate("email@email.com", 2, AEROSPACE_ENGINEERING);
    User copy = UserCopy(user);
    User other = UserCreate("aemail@email.com", 4, CHEMICAL_ENGINEERING);
    ASSERT_TEST(UserGetFaculty(user) == AEROSPACE_ENGINEERING);
    ASSERT_TEST(UserGetFaculty(other) == CHEMICAL_ENGINEERING);
    ASSERT_TEST(UserGetFaculty(user) == UserGetFaculty(copy));
    UserDestroy(user);
    UserDestroy(copy);
    UserDestroy(other);
    return true;
}
int main() {
    RUN_TEST(CreateTest);
    RUN_TEST(DestroyTest);
    RUN_TEST(CopyTest);
    RUN_TEST(CompareTest);
    RUN_TEST(GetLevelTest);
    RUN_TEST(GetEmailTest);
    RUN_TEST(GetFacultyTest);
    printf("Done! \n");
    return 0;
}