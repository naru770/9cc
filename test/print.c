#include <stdio.h>

int print(int x) {
    printf("%d\n", x);
}

int assert(int x, int y) {
    if (x == y)
        printf("OK: %d\n", x);
    else
        printf("ERROR: %d != %d\n", x, y);
}