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
    assert(FacultyGetName(faculty) == CIVIL_ENGINEERING);
    FacultyDestroy(faculty);
    return true;
}
static bool testFacultyDestroy() {
    Faculty faculty = FacultyCreate(CIVIL_ENGINEERING);
    FacultyDestroy(faculty);
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
    FacultySetProfit(facultya, 100);
    FacultySetProfit(facultyb, 200);
    ASSERT_TEST(FacultyProfitCompare(facultya, facultyb) < 0);
    FacultyDestroy(facultya);
    FacultyDestroy(facultyb);
    return true;
}

int main (int argv, char** arc) {
    RUN_TEST(testFacultyCreate);
    RUN_TEST(testFacultyDestroy);
    RUN_TEST(testFacultyCompare);
    RUN_TEST(testFacultyProfitCompare);
    printf("Done! \n");
}