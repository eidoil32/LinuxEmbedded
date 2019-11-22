//
// Created by ido on 21/11/2019.
//

#include "Miscellaneous.h"
#include <ctype.h>

bool file_exist (char *filename) {
    return fopen(filename, "r") != NULL;
}

char *toUpper(char *str) {
    char *s = str;

    for (; *s; s++)
        *s = toupper(*s);

    return s;
}