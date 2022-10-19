int foo(int n) {
    if (n == 0)
        return 0;
    else if (n == 1)
        return 1;
    else
        return foo(n - 1) + foo(n - 2);
}

int sum(int a, int b, int c, int d, int e, int f) {
    return a + b + c + d + e + f;
}

int main() {
    assert(foo(10), 55);

    assert(sum(1, 2, 3, 4, 5, 6), 21);

    int a;
    int b;
    a = 99;
    b = 10;
    assert(a + b, 109);
        
    return 0;
}