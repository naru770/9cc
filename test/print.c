#include <stdio.h>

int print(int x) {
    printf("OK: %d\n", x);
}

int eprint(int x) {
    printf("ERROR: %d\n", x);
}

int test(int x, int y) {
    if (x == y)
        printf("OK: %d\n", x);
    else
        printf("ERROR\n");
}