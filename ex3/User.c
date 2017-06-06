#include "User.h"


struct user_t {
    char* email;
    TechnionFaculty faculty;
    int skill_level;
};

User UserCreate(char* email, int skill_level, TechnionFaculty faculty) {
    if(email == NULL || StringOccurencesOfChar(email, '@') != 1 || skill_level <
        1 || skill_level > 10 || faculty < 0 || faculty > UNKNOWN) {
        return NULL;        
    }
    User user = malloc(sizeof(*user));
    if(user == NULL) {
        return NULL;
    }
    if(StringOccurencesOfChar(email, '@') != 1) {
        free(user);
        return NULL;
    }
    user -> email = StrDuplicate(email);
    if(user -> email == NULL) {
        free(user);
        return NULL;
    }
    user -> skill_level = skill_level;
    user -> faculty = faculty;
    return user;
}

void UserDestroy(void* user) {
    if(user == NULL) {
        return;
    }
    User temp = (User)user;
    free(temp -> email);
    free(temp);
    return;
}

void* UserCopy(void* user) {
    if(user == NULL) {
        return NULL;
    }
    User temp = (User)user;
    User new_user = malloc(sizeof(*new_user));
    if(new_user == NULL) {
        return NULL;
    }
    new_user -> email = StrDuplicate(temp -> email);
    if(new_user -> email == NULL) {
        free(new_user);
        return NULL;
    }
    new_user -> skill_level = temp -> skill_level;
    new_user -> faculty = temp -> faculty;
    return (void*)new_user;
}

int UserCompare(void* user1, void* user2) {
    assert(user1 != NULL && user2 != NULL);
    User temp1 = (User)user1;
    User temp2 = (User)user2;
    return(strcmp(temp1 -> email, temp2 -> email));
}

int UserGetLevel(User user) {
    if(user == NULL) {
        return 0;
    }
    return(user -> skill_level);
}

char* UserGetEmail(User user) {
    if(user == NULL) {
        return NULL;
    }
    return(user -> email);
}

TechnionFaculty UserGetFaculty(User user) {
    if(user == NULL) {
        return -1;  
    }
    return(user -> faculty);
}
