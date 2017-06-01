#ifndef USER_H
#define USER_H
#include "set.h"
#include "list.h"
#include "mtm_ex3.h"
#include <string.h>
#include "Utils.h"
#include <assert.h>

typedef struct user_t *User;
User UserCreate(char* email, int skill_level, TechnionFaculty faculty);
void UserDestroy(void* user);
void* UserCopy(void* user);
int UserCompare(void* user1, void* user2);
int UserGetLevel(User user);
char* UserGetEmail(User user);
TechnionFaculty UserGetFaculty(User user);
#endif