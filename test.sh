#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./9cc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "pass✅: $input => $actual"
    else
        echo "error❌: $input => $expected expected, but got $actual"
        exit 1
    fi
}

assert 13 'test/test1.c'
assert 11 'test/test2.c'
assert 1 'test/test_if1.c'
assert 0 'test/test_if2.c'
assert 11 'test/test_if3.c'
assert 21 'test/test_else1.c'
assert 21 'test/test_elseif1.c'
assert 55 'test/test_while1.c'
assert 55 'test/test_for1.c'
assert 55 'test/test_for2.c'
assert 11 'test/test_add_assign1.c'
assert 9 'test/test_sub_assign1.c'
assert 20 'test/test_mul_assign1.c'
assert 5 'test/test_div_assign1.c'
assert 11 'test/test_pre_inc1.c'
assert 21 'test/test_pre_inc2.c'
assert 9 'test/test_pre_dec1.c'
assert 1 'test/test_post_inc1.c'
assert 1 'test/test_post_dec1.c'
assert 1 'test/test_func1.c'
assert 12 'test/test_func2.c'
assert 55 'test/test_func_fib.c'

echo OK