#include "test_utilities.h"
#include "../../Utils.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "../../mtm_ex3.h"

int main (int argv, char** arc) {
    int x = 16;
    int y = 9;
    char* str = CreateString(x, y);
    puts(str);
    return 0;
}