main() {
    a = 10;
    b = 0;

    if (a == 9) {   
        a = b;
        b = 11;
    } else {
        a = b + 1;
        b = 21;
    }

    return b;
}