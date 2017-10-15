//
// Created by zodiac on 07/08/17.
//

#ifndef PLAYGROUND_DOK_H
#define PLAYGROUND_DOK_H

#include "SparseMatrix.hpp"
#include "DNS.hpp"
#include <map>
#include "COO.hpp"

template <typename valueType>
class DOK :public SparseMatrix<valueType>{
public:
    typedef std::tuple<unsigned long, unsigned long> coordinate;
    DOK(unsigned long n);
    DOK(unsigned long row, unsigned long col);
    DOK(const DOK<valueType> &matrix);
    DOK(const DNS<valueType> &matrix);
    DOK(const COO<valueType> &matrix);
    ~DOK();


    valueType get(unsigned long row, unsigned long col) const;
    DOK<valueType> & set(unsigned long row, unsigned long col, valueType val); // set for elements not equal 0
    DOK<valueType> & remove(unsigned long row, unsigned long col);

    valueType get(coordinate key) const;
    DOK<valueType> & set(coordinate key, valueType val);
    DOK<valueType> & remove(coordinate key);

    DOK<valueType> operator * (valueType const co) ;
    // SPmV
    std::vector<valueType> operator * (std::vector<valueType> v);
//    DOK<valueType> operator * (DOK<valueType> const &matrix) ;
    DOK<valueType> operator + (DOK<valueType> const &matrix) ;

protected:
    std::map<coordinate, valueType>* _values= nullptr;
    CompressType _type=dok;

};

template <typename valueType>
DOK<valueType>::DOK(size_t n) : SparseMatrix<valueType>(n){}

template <typename valueType>
DOK<valueType>::DOK(size_t row, size_t col) :SparseMatrix<valueType>(row,col){}

template <typename valueType>
DOK<valueType>::DOK(const DOK<valueType> &matrix) :SparseMatrix<valueType>(matrix){
    this->_values = new std::map<coordinate, valueType> (*matrix._values);
}

template <typename valueType>
DOK<valueType>::DOK(const DNS<valueType> &matrix) :SparseMatrix<valueType>(matrix){
    this->~DOK();


    std::map<coordinate, valueType>  &values = *(this->_values);

    valueType** nvalues = matrix._values;
    size_t rc = matrix.getRowCount();
    size_t cc = matrix.getColCount();

    for (size_t i = 0; i < rc; i++){
        for (size_t j = 0; j < cc; j++){
            if (nvalues[i][j] - 0 > 1e-7){
                coordinate key = std::make_tuple(i,j);
                values[key] = nvalues[i][j];
            }
        }
    }
}

template <typename valueType>
DOK<valueType>::~DOK() {
    delete this->_values;
}

template <typename valueType>
DOK<valueType>& DOK<valueType>::set(size_t row, size_t col, valueType val) {
    return this->set(std::make_tuple(row,col), val);
}

template <typename valueType>
valueType DOK<valueType>::get(size_t row, size_t col) const {
    return this->get(std::make_tuple(row,col));
}

template <typename valueType>
valueType DOK<valueType>::get(coordinate key) const {
    std::map<coordinate,valueType> & value = *(this->_values);
    return value[key];

}

template <typename valueType>
DOK<valueType>& DOK<valueType>::set(coordinate key, valueType val) {
    std::map<coordinate,valueType> & value = *(this->_values);
    value[key] = val;
    return *this;
}

template <typename valueType>
DOK<valueType>& DOK<valueType>::remove(size_t row, size_t col) {
    return this->remove(std::make_tuple(row,col));
}

template <typename valueType>
DOK<valueType>& DOK<valueType>::remove(coordinate key) {
    std::map<coordinate,valueType> & value = *(this->_values);
    auto it = value.find(key);
    if (value.count(key)){
        value.erase(it);
    }

    return *this;
}


template <typename valueType>
DOK<valueType> DOK<valueType>::operator *(valueType const co) {
    DOK<valueType> *RET = new DOK<valueType>(*this);

    for(auto it : *(RET->_values))
        it->second *= co;

    return *RET;
}

template <typename valueType>
DOK<valueType> DOK<valueType>::operator +(DOK<valueType> const &matrix) {
    DOK<valueType> &matrix_1 = *this;
    DOK<valueType> *n = new DOK<valueType>(matrix);
    DOK<valueType> &matrix_2 = *n;
    if (matrix_1.getColCount() != matrix_2.getColCount() || matrix_1.getRowCount() != matrix_2.getRowCount()){
        // throw error
    }

    for (auto it : *(this->_values)){
        coordinate key = it->first;
        valueType val_1 = it->second;
        valueType val_2 = matrix_2._values[key];
        valueType val = val_1 + val_2;

        if (val ==valueType() && val_2 !=valueType()){
            matrix_2.remove(key);
        } else {

            matrix_2.set(key, val);
        }
    }

    return matrix_2;

}

template <typename valueType>
std::vector<valueType> DOK<valueType>::operator*(std::vector<valueType> v) {

    if (this->getColCount() != v.size()){
//        return NULL;
    }

    size_t row = this->getRowCount();

    std::vector<valueType> RET(row,valueType(0));
    std::map<coordinate, valueType> &values = *(this->_values);
    for(auto &kv:values){
        coordinate const &coo =kv.first;
        valueType &val = kv.second;

        size_t const &c = std::get<1>(coo);
        size_t const &r = std::get<0>(coo);

        RET[r] += val*v[c];
    }

    return RET;
}

template <typename valueType>
DOK<valueType>::DOK(const COO<valueType> &matrix) :SparseMatrix<valueType>(matrix){


    std::vector<valueType> &values = *(matrix._values);
    std::vector<size_t> &rowIndex = *(matrix._rowIndex);
    std::vector<size_t> &colIndex = *(matrix._colIndex);
    size_t length = values.size();

    if (this->_values == nullptr){
        this->_values = new std::map<coordinate, valueType>;
    }

    for(size_t i=0; i<length; i++){
        size_t tr = rowIndex[i];
        size_t tc = colIndex[i];
        valueType val = values[i];

        this->set(tr,tc,val);
    }

}


#endif //PLAYGROUND_DOK_H
