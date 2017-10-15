//
// Created by zodiac on 30/04/17.
//
#include <cstdio>
#include <iostream>
#include "intalu.hpp"
int main()
{
    const int xl = 0, xu = 5;
    const int yl = 0, yu = 6;
    const int zl = 0, zu = 7;
    // x + (y - 2) * (z - 3) 0~5 0~6 0~7
    typedef ADD<
                VAR<1,'x',BOUND<xl,xu>>,
                MUL<
                        SUB<VAR<2,'y',BOUND<yl,yu>>,LIT<2>>,  SUB<VAR<3,'z',BOUND<zl,zu>>, LIT<3>>
                >
            > expr;

    int varAmount = 3;
    int test[][varAmount] = {{2,4,1},{5,6,7},{6,7,8}};
    int group = sizeof(test)/(varAmount*sizeof(int));
    int results[group];

    printf("%d <= x+(y-2)*(z-3) <= %d \t where %d<=x<=%d \t %d<=y<=%d \t  %d<=y<=%d \n",expr::lb, expr::ub, xl,xu,yl,yu,zl,zu);

    for (int i = 0; i<group; ++i){
        try {
            int *values = test[i];
            int result = expr::eval(values);
            printf("x+(y-2)*(z-3) = %d \t when x = %d, y = %d, z = %d\n", result,values[0],values[1],values[2]);
            results[i] = result;

        } catch (outOfBoundException &e){
            printf("%s",e.what());
        }
    }


}