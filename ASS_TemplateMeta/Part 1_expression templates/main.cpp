//
// Created by zodiac on 30/04/17.
//
#include <cstdio>
#include "intalu.hpp"
int main()
{
    // Equation: (x + 4)(x - 7)
    Variable x('x');
    Number num4(4);
    Number num7(7);
    auto eq = (x+num4)*(x-num7);
    printf("Test (x + 4)(x - 7)\n");
    printf("x = %d. Output = %d\n", 4, eq.eval(4));
    printf("x = %d. Output = %d\n", -10, eq.eval(-10));
    printf("x = %d. Output = %d\n", 6, eq.eval(6));
    printf("x = %d. Output = %d\n", 7, eq.eval(7));
}