//
// Created by zodiac on 07/08/17.
//

#ifndef PLAYGROUND_DNS_H
#define PLAYGROUND_DNS_H


#include "SparseMatrix.hpp"
#include "COO.hpp"

template <typename valueType>
class DNS : public SparseMatrix<valueType> {
public:
    DNS(size_t n);
    DNS(size_t row, size_t col);
    DNS(const DNS<valueType> &matrix);
    DNS(const COO<valueType> &matrix);
    DNS(const valueType **array, size_t row, size_t col);
    ~DNS();

    valueType get(size_t row, size_t col) const;
    DNS<valueType> & set(size_t row, size_t col, valueType val); // set for elements not equal 0

    // SPmV
    std::vector<valueType> operator * (std::vector<valueType> v);

//protected:
    valueType** _values;
    CompressType _type=dns;
};


template <typename valueType>
DNS<valueType>::DNS(size_t n) :SparseMatrix<valueType>(n){
    this->DNS(n,n);
}

template <typename valueType>
DNS<valueType>::DNS(size_t row, size_t col) :SparseMatrix<valueType>(row,col){
    this->_values = new valueType* [row];
    for (size_t r=0; r<row;r++)
        this->_values[r] = new valueType [col];
}

template <typename valueType>
DNS<valueType>::~DNS() {
    for (unsigned int i=0;i<this->_rowCount;i++){
//        for (unsigned int j=0;i<this->_colCount;j++){
//            delete _values[i][j];
//        }
        delete [] _values[i];
    }
    delete [] this->_values;
    this->_values = nullptr;
}

template <typename valueType>
DNS<valueType>::DNS(const DNS<valueType> &matrix) :SparseMatrix<valueType>(matrix){
    this->~DNS();
    size_t row = matrix.getRowCount();
    size_t col = matrix.getColCount();
    this->DNS(col,row);
    for (size_t i =0; i<row; i++)
        for (size_t j =0; j<col; j++)
            this->_values[i][j] = matrix._values[i][j];
}

template <typename valueType>
DNS<valueType>::DNS(const valueType **array, size_t row, size_t col) :SparseMatrix<valueType>(row,col){
    this->~DNS();
    this->DNS(row,col);
    for (size_t i =0; i<row; i++)
        for (size_t j =0; j<col; j++)
            this->_values[i][j] = array[i][j];
}

template <typename valueType>
DNS<valueType>& DNS<valueType>::set(size_t row, size_t col, valueType val) {
    this->_values[row][col] = val;
    return *this;
}

template <typename valueType>
valueType DNS<valueType>::get(size_t row, size_t col) const {
    return this->_values[row][col];
}

template <typename valueType>
DNS<valueType>::DNS(const COO<valueType> &matrix):SparseMatrix<valueType>(matrix) {
    size_t tmp_r = this->getRowCount();
    size_t tmp_c = this->getColCount();

//    std::cout<<"the con in DNS created "<<tmp_r<<" rows and "<<tmp_c<<" cols"<<std::endl;
    this->_values = new valueType* [tmp_r];
    for (size_t r=0; r<tmp_r;r++)
        this->_values[r] = new valueType [tmp_c];

    std::vector<valueType> &values = *(matrix._values);
    std::vector<size_t> &rowIndex = *(matrix._rowIndex);
    std::vector<size_t> &colIndex = *(matrix._colIndex);
    size_t length = values.size();


    for(size_t i=0; i<length; i++){
        size_t tr = rowIndex[i];
        size_t tc = colIndex[i];
        valueType val = values[i];

        this->_values[tr][tc] = val;
    }

}

template <typename valueType>
std::vector<valueType> DNS<valueType>::operator*(std::vector<valueType> v) {
    size_t row = this->getRowCount();
    size_t col = this->getColCount();

    std::vector<valueType> RET(row,valueType(0));

    for (size_t i =0; i<row; i++){
        for (size_t j=0;j<col;j++){
            if (this->_values[i][j] == valueType(0))
                continue;
            RET[i] = this->_values[i][j] * v[j];
        }
    }

    return RET;
}


#endif //PLAYGROUND_DNS_H
