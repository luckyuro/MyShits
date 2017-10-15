//
// Created by zodiac on 30/04/17.
//
#include <cstdio>
#include <iostream>
#include "intalu.hpp"
int main()
{
    //lower bound and upper bound
    const int lb = 0;
    const int ub = 5;

    //(x + 3) * (x + 5) with bounds (0,5)
    typedef MUL<
                ADD<VAR<BOUND<lb,ub>>,LIT<3>>,
                ADD<VAR<BOUND<lb,ub>>,LIT<5>>
               > expr;

    printf("%d <= (x+3)*(x+5) <= %d \t where %d<=x<=%d \n",expr::lb, expr::ub, lb, ub);

    int test[] = {10,1,2,3,4,5,6};
    for (int i = 0; i<sizeof(test)/sizeof(int); ++i){
        int var = test[i];
        try {
            int answer = expr::eval(var);
            printf("(x+3)*(x+5) = %d \t when x = %d \n", answer, var);
        } catch(std::exception &e){
            printf("(x+3)*(x+5) evaluate failed \t x should between %d and %d where x is %d\n", lb,ub,var);
        }
    }



}