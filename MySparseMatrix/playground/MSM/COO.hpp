//
// Created by zodiac on 07/08/17.
//

#ifndef PLAYGROUND_COO_H
#define PLAYGROUND_COO_H


#include "SparseMatrix.hpp"
#include <vector>

template <typename valueType>
class COO : public SparseMatrix<valueType>{
public:
    COO(size_t n);
    COO(size_t row, size_t col);
    COO(const COO<valueType> &matrix);
//    COO(const DNS<valueType> &matrix);
    ~COO();


    valueType get(size_t row, size_t col) const;
    COO<valueType> & set(size_t row, size_t col, valueType val); // set for elements not equal 0
    COO<valueType> & remove(size_t row, size_t col);


    COO<valueType> operator * (valueType const co) ;
    // SPmV
    std::vector<valueType> operator * (std::vector<valueType> v);
//    COO<valueType> operator * (COO<valueType> const &matrix_2) ;
//    friend COO<valueType> operator * (valueType const co, COO<valueType> const &matrix) ;
    COO<valueType> operator + (COO<valueType> const &matrix_in) ;

//protected:
    std::vector<valueType> *_values;
    std::vector<size_t> *_rowIndex;
    std::vector<size_t> *_colIndex;

    CompressType _type = coo;
};


template <typename valueType>
COO<valueType>::COO(size_t n):SparseMatrix<valueType>(n) {
    size_t size = n;
    this->_values = new std::vector<valueType>;
    this->_rowIndex = new std::vector<size_t >;
    this->_colIndex = new std::vector<size_t >;
}

template <typename valueType>
COO<valueType>::COO(size_t row, size_t col):SparseMatrix<valueType>(row,col) {
    this->_values = new std::vector<valueType>;
    this->_rowIndex = new std::vector<size_t >;
    this->_colIndex = new std::vector<size_t >;
}

template <typename valueType>
COO<valueType>::COO(const COO<valueType> &matrix):SparseMatrix<valueType>(matrix) {
    this->_values = new std::vector<valueType>(*matrix._values);
    this->_colIndex = new std::vector<size_t >(*matrix._colIndex);
    this->_rowIndex = new std::vector<size_t >(*matrix._rowIndex);
}

//template <typename valueType>
//COO<valueType>::COO(const DNS<valueType> &matrix) :SparseMatrix<valueType>(matrix){
//    this->~COO();
//
//
//    std::vector<valueType>  &values = *(this->_values);
//    std::vector<size_t> & rowIndex = *(this->_rowIndex);
//    std::vector<size_t> & colIndex = *(this->_colIndex);
//
//    valueType** nvalues = matrix._values;
//    size_t rc = matrix.getRowCount();
//    size_t cc = matrix.getColCount();
//
//    for (size_t i = 0; i < rc; i++){
//        for (size_t j = 0; j < cc; j++){
//            if (nvalues[i][j] - 0 > 1e-7){
//                rowIndex.push_back(i);
//                colIndex.push_back(j);
//                values.push_back(nvalues[i][j]);
//            }
//        }
//    }
//}

template <typename valueType>
valueType COO<valueType>::get(size_t row, size_t col) const {
    this->ValidateCoordinate(row, col);
    std::vector<valueType> & values = *(this->_values);
    std::vector<size_t> & rowIndex = *(this->_rowIndex);
    std::vector<size_t> & colIndex = *(this->_colIndex);

    size_t length = values.size();
    for (size_t i=0; i<length; i++){
        if(rowIndex.at(i) == row && colIndex.at(i) == col){
            return values.at(i);
        }
    }
    return 0;
}

template <typename valueType>
COO<valueType>::~COO() {
    delete this->_values;
    delete this->_rowIndex;
    delete this->_colIndex;
}

template <typename valueType>
COO<valueType>& COO<valueType>::set(size_t row, size_t col, valueType val) {
    this->ValidateCoordinate(row, col);
    std::vector<valueType> & values = *(this->_values);
    std::vector<size_t> & rowIndex = *(this->_rowIndex);
    std::vector<size_t> & colIndex = *(this->_colIndex);

    size_t length = values.size();
    for (size_t i=0; i<length; i++){
        if(rowIndex.at(i) == row && colIndex.at(i) == col){
            values.at(i) = val;
            return *this;
        }
    }
    rowIndex.push_back(row);
    colIndex.push_back(col);
    values.push_back(val);
    return *this;
}

template <typename valueType>
COO<valueType>& COO<valueType>::remove(size_t row, size_t col) {
    this->ValidateCoordinate(row, col);
    std::vector<valueType> & values = *(this->_values);
    std::vector<size_t> & rowIndex = *(this->_rowIndex);
    std::vector<size_t> & colIndex = *(this->_colIndex);

    size_t length = values.size();
    for (size_t i=0; i<length; i++){
        if(rowIndex.at(i) == row && colIndex.at(i) == col){
            rowIndex.at(i) = rowIndex.at(length-1); rowIndex.pop_back();
            colIndex.at(i) = colIndex.at(length-1); colIndex.pop_back();
            values.at(i) = values.at(length-1); values.pop_back();

            return *this;
        }
    }

    return *this;

}

template <typename valueType>
COO<valueType> COO<valueType>::operator *(valueType const co) {
    COO<valueType> *RET = new COO<valueType>(*this);

    for(auto it : *(RET->_values))
        *(it) *= co;

    return *RET;
}

//template <typename valueType>
//COO<valueType> operator *(valueType const co, COO<valueType> const &matrix) {
//    COO<valueType> *RET = new COO<valueType>(matrix);
//
//    for(auto it : *(RET->_values))
//        it *= co;
//
//    return *RET;
//}

template <typename valueType>
COO<valueType> COO<valueType>::operator +(COO<valueType> const &matrix_in) {
    COO<valueType> &matrix_1 = *this;
    COO<valueType> *n = new COO<valueType>(matrix_in);
    COO<valueType> &matrix_2 = *n;
    if (matrix_1.getColCount() != matrix_2.getColCount() || matrix_1.getRowCount() != matrix_2.getRowCount()){
        // throw error
    }

    size_t nnz_1 = this->_values->size();

    for (size_t i =0 ; i < nnz_1; i++){
        size_t r = this->_rowIndex->at(i);
        size_t c = this->_colIndex->at(i);
        valueType val_1 = this->_values->at(i);
        valueType val_2 = matrix_2.get(r, c);
        valueType val = val_1 + val_2;

        if (val == valueType() && val_2 != valueType()){
            matrix_2.remove(r,c);
        } else {

            matrix_2.set(r,c,val);
        }
    }

    return matrix_2;

}

template <typename valueType>
std::vector<valueType> COO<valueType>::operator*(std::vector<valueType> v) {

    if (this->getColCount() != v.size()){
//         return nullptr;
    }

    size_t row = this->getRowCount();
    size_t col = this->getColCount();
    size_t length = this->_values->size();


    std::vector<valueType> RET(row,valueType(0));

    for (size_t i=0; i<length; i++){
        size_t r = this->_rowIndex->at(i);
        size_t c = this->_colIndex->at(i);
        valueType val = this->_values->at(i);

        RET[r] += val*v[c];
    }

    return RET;
}

#endif //PLAYGROUND_COO_H
