//
// Created by zodiac on 06/08/17.
//

// the base of csc csr bsr
// index based on 0
// commonlly 3 vector
// {1,2,3,4,5....} all values
// {} store row/col index in the matrix
// {} store the start index in the upper matrix


#ifndef PLAYGROUND_CS_BASE_H
#define PLAYGROUND_CS_BASE_H

#include "SparseMatrix.hpp"
#include <vector>
#include "DNS.hpp"

template <typename valueType>
class CS_BASE : public SparseMatrix<valueType>{
public:
    CS_BASE(size_t n);
    CS_BASE(size_t row, size_t col);
    CS_BASE(const CS_BASE<valueType> &matrix);
    CS_BASE(const DNS<valueType> &matrix);
    ~CS_BASE();

    valueType get(size_t compress_i, size_t all_i) const;
    CS_BASE<valueType> & set(size_t compress_i, size_t all_i, valueType val); // set for elements not equal 0
    CS_BASE<valueType> & remove(size_t compress_i, size_t all_i);

protected:
    std::vector<valueType> *_values= nullptr;
    std::vector<size_t> *_all_index= nullptr, *_c_index= nullptr;

};


template <typename valueType>
CS_BASE<valueType>::CS_BASE(size_t n) : SparseMatrix<valueType>(n){
    std::vector<valueType> &values = *(this->_values);
    std::vector<size_t > &all_index= *(this->_all_index);
    std::vector<size_t > &c_index = *(this->_c_index);

    values.reserve((size_t)(0.6*n*n));
    all_index.reserve((size_t) (0.6*n*n));
    c_index.reserve((size_t) (n+2));
};

template <typename valueType>
CS_BASE<valueType>::CS_BASE(size_t row, size_t col):SparseMatrix<valueType>(row, col){
    std::vector<valueType> &values = *(this->_values);
    std::vector<size_t > &all_index= *(this->_all_index);

    values.reserve((size_t)(0.6*row*col));
    all_index.reserve((size_t)(0.6*row*col));
}

template <typename valueType>
CS_BASE<valueType>::CS_BASE(const CS_BASE<valueType> &matrix) : SparseMatrix<valueType>(matrix){
    this->~CS_BASE();
    this->_values = new std::vector<valueType>(*(matrix._values));
    this->_c_index = new std::vector<valueType>(*(matrix._c_index));
    this->_all_index = new std::vector<valueType>(*(matrix._all_index));
}

template <typename valueType>
CS_BASE<valueType>::CS_BASE(const DNS<valueType> &matrix) :SparseMatrix<valueType>(matrix){
    if(this->_values == nullptr){
        this->_values = new std::vector<valueType>;
    }
    if(this->_all_index == nullptr){
        this->_all_index = new std::vector<size_t >;
    }
    if(this->_c_index == nullptr){
        this->_c_index = new std::vector<size_t >;
    }
}

template <typename valueType>
CS_BASE<valueType>::~CS_BASE() {
    delete this->_values;
    delete this->_all_index;
    delete this->_c_index;
}

template <typename valueType>
valueType CS_BASE<valueType>::get(size_t compress_i, size_t all_i) const {
    std::vector<valueType> &values = *(this->_values);
    std::vector<size_t > &all_index= *(this->_all_index);
    std::vector<size_t > &c_index = *(this->_c_index);

    size_t start = c_index.at(compress_i);
    size_t end = c_index.at(compress_i+1);
    for (size_t i = start; i<end; i++){
        if (all_index.at(i) == all_i){
            return values.at(i);
        }
        if (all_index.at(i) > all_i){
            break;
        }
    }

    return 0;
}

template <typename valueType>
CS_BASE<valueType>& CS_BASE<valueType>::set(size_t compress_i, size_t all_i, valueType val) {
    std::vector<valueType> &values = *(this->_values);
    std::vector<size_t > &all_index= *(this->_all_index);
    std::vector<size_t > &c_index = *(this->_c_index);

    size_t start = c_index.at(compress_i);
    size_t end = c_index.at(compress_i+1);

    auto value_it= values.begin();
    auto index_it= all_index.begin();
    for (size_t i = start; i<end; i++){
        if (all_index.at(i) == all_i){
            values.at(i) = val;
            return *this;
        }
        if (all_index.at(i) > all_i){
            end = i;
            break;
        }
    }

    value_it += end;
    index_it += end;

    values.insert(value_it, val);
    all_index.insert(index_it, all_i);

    for (auto i=c_index.begin()+compress_i+1; i != c_index.end();i++){
        *i += 1;
    }

    return *this;
}

template <typename valueType>
CS_BASE<valueType>& CS_BASE<valueType>::remove(size_t compress_i, size_t all_i) {
    std::vector<valueType> &values = *(this->_values);
    std::vector<size_t > &all_index= *(this->_all_index);
    std::vector<size_t > &c_index = *(this->_c_index);

    size_t start = c_index.at(compress_i);
    size_t end = c_index.at(compress_i+1);

    auto value_it= values.begin();
    auto index_it= all_index.begin();


    for (size_t i = start; i<end; i++){
        if (all_index.at(i) == all_i){
            all_index.erase(all_index.begin()+i);
            values.erase(values.begin()+i);

            for (auto j=c_index.begin()+compress_i+1; j != c_index.end();i++){
                *j -= 1;
            }

            break;
        }
        if (all_index.at(i) > all_i){
            break;
        }
    }

    return *this;
}

#endif //PLAYGROUND_CS_BASE_H
