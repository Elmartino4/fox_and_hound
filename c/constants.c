#include "constants.h"
#include <stdio.h>

int search_for_collision(coord* positions, char length, coord find) {
    // simple linear search for "inputCoord" in the "characterPositions" array
    for (char i = 0; i < length; i++)
    {
        if (positions[i] == find)
            return i;
    }

    return -1;
}

void flush() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void arrayCopy(char* arrIn, char* arrOut, int length) {
    for (size_t i = 0; i < length; i++)
    {
        arrOut[i] = arrIn[i];
    }
}