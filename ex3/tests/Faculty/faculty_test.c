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
    faculty = FacultyCreate(CIVIL_ENGINEERING);
    assert(FacultyGetName(faculty) == CIVIL_ENGINEERING);
    FacultyDestroy(faculty);
    return true;
}
static bool testFacultyDestroy() {
    faculty = FacultyCreate(NULL);
    FacultyDestroy(faculty);
    return true;
}
static bool testFacultyCompare() {
    facultya = FacultyCreate(PHYSICS);
    facultyb = FacultyCreate(MATHEMATICS);
    ASSERT_TEST(FacultyCompare(facultya, facultyb) > 0);
    FacultyDestroy(facultya);
    FacultyDestroy(facultyb);
    return true;
}
static bool testFacultyProfitCompare() {
    facultya = FacultyCreate(PHYSICS);
    facultyb = FacultyCreate(MATHEMATICS);
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