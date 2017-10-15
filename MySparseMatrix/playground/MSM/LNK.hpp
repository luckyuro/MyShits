//
// Created by zoLNKc on 08/08/17.
//

#ifndef PLAYGROUND_LNK_H
#define PLAYGROUND_LNK_H

#include "SparseMatrix.hpp"
#include "DNS.hpp"

template <typename T>
class LNK : public SparseMatrix<T>{
public:
    LNK(const LNK<T> &matrix);
    LNK(const DNS<T> &matrix);
    ~LNK();


    T get(unsigned long row, unsigned long col) const;
    LNK<T> & set(unsigned long row, unsigned long col, T val); // set for elements not equal 0
    LNK<T> & remove(unsigned long row, unsigned long col);

protected:
    std::vector<T> ** _values;
    std::vector<unsigned long> *_distance;
    unsigned long _r, _c;

    CompressType _type = lnk;

};


#endif //PLAYGROUND_LNK_H
