//
// Created by zoSKYc on 08/08/17.
//

#ifndef PLAYGROUND_SKY_H
#define PLAYGROUND_SKY_H

#include "SparseMatrix.hpp"
#include "DNS.hpp"

template <typename T>
class SKY : public SparseMatrix<T>{
public:
    SKY(const SKY<T> &matrix);
    SKY(const DNS<T> &matrix);
    ~SKY();


    T get(unsigned long row, unsigned long col) const;
    SKY<T> & set(unsigned long row, unsigned long col, T val); // set for elements not equal 0
    SKY<T> & remove(unsigned long row, unsigned long col);

protected:
    std::vector<T> ** _values;
    std::vector<unsigned long> *_distance;
    unsigned long _r, _c;

    CompressType _type = sky;

};


#endif //PLAYGROUND_SKY_H
