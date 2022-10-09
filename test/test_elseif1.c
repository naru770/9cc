main() {
    a = 10;
    b = 0;

    if (a == 9) {   
        a = b;
        b = 11;
    } else if (a == 8) {
        a = b + 1;
        b = 1;
    } else {
        b = 21;
    }

    return b;
}