//
// Created by zodiac on 06/07/17.
//

#ifndef MSM_SPARSEMATRIX_H
#define MSM_SPARSEMATRIX_H

#define _my0 1e-7
#include <vector>
#include <array>
#include <map>
#include <algorithm>
//#include "DNS.h"

enum CompressType {csr,csc,bsr,coo,dok,dia,sky,jag,lnk,dns,ell,hyb,lay};

// the base class, some functions and attributes should include in all types
template <typename valueType>
class SparseMatrix {
public:
    // construction functions
    SparseMatrix(size_t n);
    SparseMatrix(size_t row, size_t col);
    SparseMatrix(const SparseMatrix<valueType> &matrix);
//    SparseMatrix(const DNS<valueType> &matrix);

    // SparseMatrix(const SparseMatrix<valueType> &m);
    // SparseMatrix<valueType> &operator = (const SparseMatrix<valueType> &m);
    //virtual ~SparseMatrix()=0;

    // get the size of the matrix
    virtual size_t getRowCount() const;
    virtual size_t getColCount() const;

    // get and set elements in the matrix
//    virtual valueType get(size_t row, size_t col) const=0;
//    virtual SparseMatrix<valueType>& set (size_t row, size_t col, valueType val) =0;
//    virtual SparseMatrix<valueType>& remove (size_t row, size_t col) =0;

    virtual std::vector<valueType> operator *(std::vector<valueType> v) =0;
    //operations
//    virtual SparseMatrix<valueType>& Multiply(SparseMatrix<valueType> const &matrix) const =0;
//    virtual SparseMatrix<valueType> operator *(SparseMatrix<valueType> const &matrix) const =0;
//
//    virtual SparseMatrix<valueType> Add(SparseMatrix<valueType> const &matrix) const =0;
//    virtual SparseMatrix<valueType> operator +(SparseMatrix<valueType> const &matrix) const =0;
//
//    virtual SparseMatrix<valueType> Scale(valueType co) const=0;
//    virtual SparseMatrix<valueType> operator *(SparseMatrix<valueType> const &matrix, valueType co) =0;
//    virtual SparseMatrix<valueType> operator *(valueType co, SparseMatrix<valueType> const &matrix) =0;

//    virtual SparseMatrix<valueType>& from_dense(const valueType** array) =0;
//    virtual valueType** to_dense() const=0;


protected:
    size_t _rowCount, _colCount; // the shape of the matrix,
    CompressType _type ; //how it compressed eg. csc, csr, dia......
    size_t _nnz; //number of none zeros
    bool ValidateCoordinate(size_t row, size_t col) const; // whether a value is validate in the matrix
    void CheckShape(size_t row, size_t col) const;

};

template <typename valueType>
size_t SparseMatrix<valueType>::getColCount() const { return this->_colCount;}

template <typename valueType>
size_t SparseMatrix<valueType>::getRowCount() const { return this->_rowCount;}

template <typename valueType>
SparseMatrix<valueType>::SparseMatrix(size_t n):_rowCount(n),_colCount(n){
    CheckShape(n,n);
}

template <typename valueType>
SparseMatrix<valueType>::SparseMatrix(size_t row, size_t col):_rowCount(row),_colCount(col) {
    CheckShape(row,col);
}

template <typename valueType>
SparseMatrix<valueType>::SparseMatrix(const SparseMatrix<valueType> &matrix){
    this->_rowCount = matrix.getColCount();
    this->_colCount = matrix.getColCount();
}

template <typename valueType>
bool SparseMatrix<valueType>::ValidateCoordinate(size_t row, size_t col) const {
    return (row<_rowCount && col<_colCount);
}

template <typename valueType>
void SparseMatrix<valueType>::CheckShape(size_t row, size_t col) const {
    if (row<1 || col<1){
        //throw error here
    }
}

#endif //PLAYGROUND_SPARSEMATRIX_H
