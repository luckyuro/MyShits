#include <iostream>
#include "Metric.hpp"
#include <string>
#include <fstream>


int main() {
    std::ifstream matrices;
    matrices.open("matrices");

    std::string name, path;
    while(std::getline(matrices, name)){
        std::getline(matrices, path);


        std::cout<<name<<std::endl;
        std::cout<<path<<std::endl;

            Metric<double> metric;
//            std::string s("swang1.mtx");
            metric.test(name, path);
//            std::cout << s << std::endl;

    }


    return 0;
}