//
// Created by zodiac on 11/07/17.
//

#ifndef PLAYGROUND_CSC_H
#define PLAYGROUND_CSC_H

#include "CS_BASE.hpp"
#include "DNS.hpp"

template <typename valueType>
class CSC: public CS_BASE<valueType>{
public:
    CSC(size_t n);
    CSC(size_t row, size_t col);
    CSC(const DNS<valueType> &matrix);

    ~CSC() =default;

    CSC(const CSC<valueType> &matrix);
    CSC<valueType> &operator = (const CSC<valueType> &matrix);

    valueType get(size_t row, size_t col) const;
    CSC<valueType> & set(size_t row, size_t col, valueType val);
    CSC<valueType> & remove(size_t row, size_t col);


    CSC<valueType> operator *(valueType const co) ;
    // SPmV
    std::vector<valueType> operator * (std::vector<valueType> v);
//    CSC<valueType> operator *(CSC<valueType> const &matrix) ;
    CSC<valueType> operator +(CSC<valueType> const &matrix) ;

protected:
    CompressType _type= csc;
    std::vector<size_t> *_rowIndex, *_colIndex;
};


template <typename valueType>
CSC<valueType>::CSC(size_t n):CS_BASE<valueType>(n) {
    this->_colIndex = this->_c_index;
    this->_rowIndex = this->_all_index;
}

template <typename valueType>
CSC<valueType>::CSC(size_t row, size_t col):CS_BASE<valueType>(row,col){
    this->_colIndex = this->_c_index;

    std::vector<size_t> &colIndex = *(this->_colIndex);
    colIndex.reserve(col+2);
    this->_rowIndex = this->_all_index;
}


template <typename valueType>
CSC<valueType>::CSC(const CSC<valueType> &matrix) : CS_BASE<valueType>(matrix) {
    this->_rowIndex = this->_all_index;
    this->_colIndex = this->_c_index;
}

template <typename valueType>
CSC<valueType>::CSC(const DNS<valueType> &matrix) : CS_BASE<valueType>(matrix) {
//    this->~CSC();

    this->_colIndex = this->_c_index;
    this->_rowIndex = this->_all_index;

    std::vector<valueType>  &values = *(this->_values);
    std::vector<size_t > & rowIndex = *(this->_all_index);
    std::vector<size_t > & colIndex = *(this->_c_index);

    valueType** nvalues = matrix._values;
    size_t rc = matrix.getRowCount();
    size_t cc = matrix.getColCount();

    size_t count = 0;
    colIndex.push_back(count);
    for (size_t j = 0; j < cc; j++){
        for (size_t i = 0; i < rc; i++){
            if (nvalues[i][j] - 0 > 1e-7){
                values.push_back(nvalues[i][j]);
                rowIndex.push_back(i);
                count++;
            }
        }
        colIndex.push_back(count);
    }

}


template <typename valueType>
CSC<valueType> & CSC<valueType>::operator=(const CSC<valueType> &matrix) {
    if(&matrix != this){
        this->CSC(matrix);
    }
    return *this;
}

template <typename valueType>
valueType CSC<valueType>::get(size_t row, size_t col) const {
    this->ValidateCoordinate(row,col);
    return CS_BASE<valueType>::get(col,row);
}

template <typename valueType>
CSC<valueType>& CSC<valueType>::set(size_t row, size_t col, valueType val) {
    this->ValidateCoordinate(row,col);
    return (CSC<valueType> &)CS_BASE<valueType>::set(col,row,val);
}

template <typename valueType>
CSC<valueType>& CSC<valueType>::remove(size_t row, size_t col) {
    this->ValidateCoordinate(row,col);
    return (CSC<valueType> &)CS_BASE<valueType>::remove(col,row);
}



//template <typename valueType>
//CSC<valueType>& CSC<valueType>::Multiply(CSC<valueType> const &matrix) {
//    if (this-> col_ != matrix.row_){
//        //throw error
//    }
//
//    CSC<valueType> result(this->col_, matrix.row_);
//
//    valueType acc;
//    for (unsigned int i = 1; i <=this->col_; i++){
//        for (unsigned int j=1; j<=matrix.row_; j++){
//            acc = valueType();
//
//            for (unsigned int k=1; k<= this->row_; k++){
//                acc += this->get(i,k) * matrix.get(k,j);
//            }
//
//            result.set(acc,i ,j);
//        }
//    }
//    return result;
//}

template <typename valueType>
CSC<valueType> CSC<valueType>::operator +(CSC<valueType> const &matrix) {
    CSC <valueType> &matrix_1 = *this;
    CSC <valueType> *n = new CSC<valueType>(matrix);
    CSC <valueType> &matrix_2 = *n;
    if (matrix_1.getColCount() != matrix_2.getColCount() || matrix_1.getRowCount() != matrix_2.getRowCount()) {
        // throw error
    }

    size_t nnz_1 = this->_values->size();

    for (size_t i = 0; i < nnz_1; i++) {
        size_t r = this->_rowIndex->at(i);
        size_t c = this->_colIndex->at(i);
        valueType val_1 = this->_values->at(i);
        valueType val_2 = matrix_2.get(r, c);
        valueType val = val_1 + val_2;

        if (val ==valueType() && val_2 !=valueType()) {
            matrix_2.remove(r, c);
        } else {

            matrix_2.set(r, c, val);
        }
    }

    return matrix_2;
}

template <typename valueType>
CSC<valueType> CSC<valueType>::operator*(const valueType co) {
    CSC<valueType> *RET = new CSC<valueType>(*this);
    for(auto it : *(RET->_values))
        *(it) *= co;

    return *RET;
}

template <typename valueType>
std::vector<valueType> CSC<valueType>::operator*(std::vector<valueType> v) {


    if (this->getColCount() != v.size()){
    //    return NULL;
    }

    size_t row = this->getRowCount();
    size_t col = this->getColCount();
    size_t length = this->_values->size();


    std::vector<valueType> RET(row,valueType(0));

    for(size_t ci =0; ci<col; ci++){
        size_t start = this->_colIndex->at(ci);
        size_t end = this->_colIndex->at(ci+1);

        for(size_t ri =start; ri<end; ri++){
            size_t g = this->_rowIndex->at(ri);
            valueType val = this->_values->at(ri);
            RET[g] += v[ci]*val;
        }

    }

    return RET;
}


#endif //PLAYGROUND_CSC_H
