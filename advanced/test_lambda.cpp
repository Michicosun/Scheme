#include <string>
#include <iostream>

#include "../test/scheme_test.h"
#include "catch.hpp"
#include "allocations_checker.h"

TEST_CASE_METHOD(SchemeTest, "SimpleLambda") {
    ExpectEq("((lambda (x) (+ 1 x)) 5)", "6");
}

TEST_CASE_METHOD(SchemeTest, "LambdaBodyHasImplicitBegin") {
    ExpectNoError("(define test (lambda (x) (set! x (* x 2)) (+ 1 x)))");
    ExpectEq("(test 20)", "41");
}

TEST_CASE_METHOD(SchemeTest, "SlowSum") {
    ExpectNoError("(define slow-add (lambda (x y) (if (= x 0) y (slow-add (- x 1) (+ y 1)))))");
    ExpectEq("(slow-add 3 3)", "6");
    ExpectEq("(slow-add 100 100)", "200");
}

TEST_CASE_METHOD(SchemeTest, "LambdaClosure") {
    ExpectNoError("(define x 1)");

    // R"EOF( is not part of the string. It is syntax for raw string literal in C++.
    // https://en.cppreference.com/w/cpp/language/string_literal
    ExpectNoError(R"EOF(
        (define range
          (lambda (x)
            (lambda ()
              (set! x (+ x 1))
              x)))
                    )EOF");

    // (define range (lambda (x) (lambda () (set! x (+ x 1)) x)))

    ExpectNoError("(define my-range (range 10))");
    ExpectEq("(my-range)", "11");
    ExpectEq("(my-range)", "12");
    ExpectEq("(my-range)", "13");

    ExpectEq("x", "1");
}

TEST_CASE_METHOD(SchemeTest, "LambdaSyntax") {
    ExpectSyntaxError("(lambda)");
    ExpectSyntaxError("(lambda x)");
    ExpectSyntaxError("(lambda (x))");
}

TEST_CASE_METHOD(SchemeTest, "DefineLambdaSugar") {
    ExpectNoError("(define (inc x) (+ x 1))");
    ExpectEq("(inc -1)", "0");

    ExpectNoError("(define (add x y) (+ x y 1))");
    ExpectEq("(add -10 10)", "1");

    ExpectNoError("(define (zero) 0)");
    ExpectEq("(zero)", "0");
}

TEST_CASE_METHOD(SchemeTest, "LambdaMultipleRecurseCalls") {
    ExpectNoError("(define (fib x) (if (< x 3) 1 (+ (fib (- x 1)) (fib (- x 2)) )))");
    ExpectEq("(fib 1)", "1");
    ExpectEq("(fib 2)", "1");
    ExpectEq("(fib 3)", "2");
    ExpectEq("(fib 7)", "13");
    ExpectEq("(fib 8)", "21");
}

TEST_CASE_METHOD(SchemeTest, "MutualCalls") {
    ExpectNoError("(define (foo x) (if (< x 2) 42 (bar (- x 1))))");
    ExpectNoError("(define (bar x) (if (< x 2) 24 (foo (/ x 2))))");
    ExpectEq("(foo 3)", "42");
    ExpectEq("(foo 6)", "24");
    ExpectEq("(bar 7)", "42");
    ExpectEq("(bar 13)", "24");
}

TEST_CASE_METHOD(SchemeTest, "LambdasShareContext") {
    ExpectNoError(R"EOF(
        (define (foo x)
            (cons
                (lambda () (set! x (+ x 1)) x)
                (lambda () (set! x (* x 2)) x)
            )
        )
    )EOF");
    ExpectNoError("(define my-foo (foo 15))");
    ExpectEq("((cdr my-foo))", "30");
    ExpectEq("((car my-foo))", "31");
    ExpectEq("((car my-foo))", "32");
    ExpectEq("((cdr my-foo))", "64");
}

TEST_CASE_METHOD(SchemeTest, "CyclicLocalContextDependencies") {
    ExpectNoError(R"EOF(
        (define (foo x)
            (define (bar) (set! x (+ (* x 2) 2)) x)
            bar
        )
    )EOF");
    ExpectNoError("(define my-foo (foo 20))");
    ExpectNoError("(define foo 1543)");
    ExpectEq("(my-foo)", "42");
}

TEST_CASE_METHOD(SchemeTest, "LambdaScopePrune") {
    alloc_checker::ResetCounters();

    for (uint32_t i = 0; i < 100; ++i) {
        std::string fn = "ahaha" + std::to_string(i);
        ExpectNoError("(define (" + fn + " x) (if (= x 0) 0 (+ 1 (" + fn + " (- x 1)))))");
        ExpectEq("(" + fn + " 1000)", "1000");
    }

    int64_t alloc_count = alloc_checker::AllocCount(),
            dealloc_count = alloc_checker::DeallocCount();

    std::cerr << "LambdaScopePrune:\n";
    std::cerr << "Allocations: " << alloc_count << "\n";
    std::cerr << "Deallocations: " << dealloc_count << "\n\n";

    REQUIRE(alloc_count - dealloc_count <= 30'000);
}
