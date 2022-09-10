#include "extra.h"

char* removeLeadingSpaces(char* str)
{
    static char str1[99];
    int count = 0, j, k;

    // Iterate String until last
    // leading space character
    while (str[count] == ' ') {
        count++;
    }

    // Putting string into another
    // string variable after
    // removing leading white spaces
    for (j = count, k = 0;
        str[j] != '\0'; j++, k++) {
        str1[k] = str[j];
    }
    str1[k] = '\0';

    return str1;
}