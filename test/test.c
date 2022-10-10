foo(n) {
    if (n == 0)
        return 0;
    else if (n == 1)
        return 1;
    else
        return foo(n - 1) + foo(n - 2);
}

main() {
    test(foo(10), 55);

        
    return 0;
}