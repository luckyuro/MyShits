//
// Created by zodiac on 06/07/17.
//

#ifndef PLAYGROUND_CSR_H
#define PLAYGROUND_CSR_H

#include "CS_BASE.hpp"
#include "DNS.hpp"

template <typename valueType>
class CSR: public CS_BASE<valueType>{
public:
    CSR(size_t n);
    CSR(size_t row, size_t col);
    CSR(DNS<valueType> &matrix);

    ~CSR() =default;

    CSR(const CSR<valueType> &matrix);
    CSR<valueType> &operator = (const CSR<valueType> &matrix);

    valueType get(size_t row, size_t col) const;
    CSR<valueType> & set(size_t row, size_t col, valueType val);
    CSR<valueType> & remove(size_t row, size_t col);

    CSR<valueType> operator *(valueType const co) ;
    // SPmV
    std::vector<valueType> operator * (std::vector<valueType> v);
//    CSR<valueType> operator *(CSR<valueType> const &matrix) ;
    CSR<valueType> operator +(CSR<valueType> const &matrix) ;

protected:
    CompressType _type= csr;
    std::vector<size_t> *_rowIndex,*_colIndex;
};


template <typename valueType>
CSR<valueType>::CSR(size_t n):CS_BASE<valueType>(n) {
    this->_rowIndex = this->_c_index;
    this->_colIndex = this->_all_index;
}

template <typename valueType>
CSR<valueType>::CSR(size_t row, size_t col):CS_BASE<valueType>(row,col){
    this->_rowIndex = this->_c_index;
    std::vector<size_t> &rowIndex = *(this->_rowIndex);
    rowIndex.reserve(col+2);
    this->_colIndex = this->_all_index;
}


template <typename valueType>
CSR<valueType>::CSR(const CSR<valueType> &matrix) : CS_BASE<valueType>(matrix) {
    this->_rowIndex = this->_c_index;
    this->_colIndex = this->_all_index;
}

template <typename valueType>
CSR<valueType>::CSR(DNS<valueType> &matrix) : CS_BASE<valueType>(matrix){
//    this->~CSR();
    this->_rowIndex = this->_c_index;
    this->_colIndex = this->_all_index;

    std::vector<valueType>  &values = *(this->_values);
    std::vector<size_t > & rowIndex = *(this->_all_index);
    std::vector<size_t > & colIndex = *(this->_c_index);

    valueType** nvalues = matrix._values;
    size_t rc = matrix.getRowCount();
    size_t cc = matrix.getColCount();

    size_t count = 0;
    rowIndex.push_back(count);
//    std::cout<<"left filling"<<std::endl;
    for (size_t i = 0; i < cc; i++){
        for (size_t j = 0; j < rc; j++){
            if (nvalues[i][j] - 0 > 1e-7){
                values.push_back(nvalues[i][j]);
                colIndex.push_back(j);
                count++;
            }
        }
        rowIndex.push_back(count);
    }

}

template <typename valueType>
CSR<valueType> & CSR<valueType>::operator=(const CSR<valueType> &matrix) {
    if(&matrix != this){
        this->CSR(matrix);
    }
    return *this;
}

template <typename valueType>
valueType CSR<valueType>::get(size_t row, size_t col) const {
    this->ValidateCoordinate(row,col);
    return CS_BASE<valueType>::get(row,col);
}

template <typename valueType>
CSR<valueType>& CSR<valueType>::set(size_t row, size_t col, valueType val) {
    this->ValidateCoordinate(row,col);
    return (CSR<valueType> &)CS_BASE<valueType>::set(row,col,val);
}

template <typename valueType>
CSR<valueType>& CSR<valueType>::remove(size_t row, size_t col) {
    this->ValidateCoordinate(row,col);
    return (CSR<valueType> &)CS_BASE<valueType>::remove(row,col);
}


//template <typename valueType>
//CSR<valueType>& CSR<valueType>::Multiply(CSR<valueType> const &matrix) {
//    if (this-> col_ != matrix.row_){
//        //throw error
//    }
//
//    CSR<valueType> result(this->col_, matrix.row_);
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
CSR<valueType> CSR<valueType>::operator*(valueType const co) {
    CSR<valueType> *RET = new CSR<valueType>(*this);
    for(auto it : *(RET->_values))
        *(it) *= co;

    return *RET;
}

template <typename valueType>
CSR<valueType> CSR<valueType>::operator +(CSR<valueType> const &matrix) {
    CSR <valueType> &matrix_1 = *this;
    CSR <valueType> *n = new CSR<valueType>(matrix);
    CSR <valueType> &matrix_2 = *n;
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
std::vector<valueType> CSR<valueType>::operator*(std::vector<valueType> v) {


    if (this->getColCount() != v.size()){
      //  return NULL;
    }

    size_t row = this->getRowCount();
    size_t col = this->getColCount();
    size_t length = this->_values->size();


    std::vector<valueType> RET(row,valueType(0));

    for(size_t ri =0; ri<row; ri++){
        size_t start = this->_rowIndex->at(ri);
        size_t end = this->_rowIndex->at(ri+1);

        for(size_t ci =start; ci<end; ci++){
            size_t g = this->_colIndex->at(ci);
            valueType val = this->_values->at(ci);
            RET[ri] += v[g]*val;
        }

    }

    return RET;
}


#endif //PLAYGROUND_CSR_H
