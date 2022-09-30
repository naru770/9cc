#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./9cc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

assert 0 "0;"
assert 12 "12;"
assert 21 "5+20-4;"
assert 41 " 12 + 34 - 5; "
assert 47 '5+6*7;'
assert 15 '5*(9-6);'
assert 4 '(3+5)/2;'
assert 1 '(-3 * 2) + 7;'
assert 0 '1 == 0;'
assert 1 '123 != 6;'
assert 0 '8 < 2;'
assert 1 '29 <= 55;'
assert 4 'a = 2; b = 4;'
assert 11 'a = 9; b = a + 2;'
assert 12 'foo = 10; bar = 2; r = foo + bar;'
assert 20 'a = 10; b = 11; return a + b - 1;'

echo OK