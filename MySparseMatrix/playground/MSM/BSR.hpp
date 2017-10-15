//
// Created by zoBSRc on 08/08/17.
//

#ifndef PLAYGROUND_BSR_H
#define PLAYGROUND_BSR_H

#include "CS_BASE.hpp"
#include "DNS.hpp"

template <typename T>
class BSR : public CS_BASE<T>{
public:
    BSR(const BSR<T> &matrix);
    BSR(const DNS<T> &matrix);
    ~BSR();


    T get(unsigned long row, unsigned long col) const;
    BSR<T> & set(unsigned long row, unsigned long col, T val); // set for elements not equal 0
    BSR<T> & remove(unsigned long row, unsigned long col);

protected:
    std::vector<T> ** _values;
    std::vector<unsigned long> *_distance;
    unsigned long _r, _c;

    CompressType _type = bsr;

};


#endif //PLAYGROUND_BSR_H
