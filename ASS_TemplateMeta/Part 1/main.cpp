//
// Created by zodiac on 10/05/17.
//
#include <cstdio>
#include "intalu.hpp"

int main()
{

    //x + (x - 2)*(x - 3)
    typedef ADD<
                VAR,
                MUL<
                        SUB<VAR,LIT<2>>, SUB<VAR,LIT<3>>
                >> eq;
    int test[] = {10,1,2,3,4,5,6,7,8,9,0,-1};
    for (int i = 0; i<sizeof(test)/sizeof(int); ++i){
        int var = test[i];
        printf("x+(x-2)*(x-3) = %d \t",eq::eval(var));
        printf("when x=%d \n",var);
    }

    return 0;
}
