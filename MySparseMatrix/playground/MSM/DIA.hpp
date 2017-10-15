//
// Created by zodiac on 07/08/17.
//

// distance       d1    d2    d3    d4
// valueType **   r1      1     2     3     4
//        r2      0     2     5     1
// number of rows is the length of main diagonal, min(row,col)


#ifndef PLAYGROUND_DIA_H
#define PLAYGROUND_DIA_H

#include "SparseMatrix.hpp"
#include "DNS.hpp"
#include "COO.hpp"
#include <set>
#include <map>

template <typename valueType>
class DIA : public SparseMatrix<valueType>{
public:
    DIA(size_t n);
    DIA(size_t row, size_t col);
    DIA(const DIA<valueType> &matrix);
    DIA(const DNS<valueType> &matrix);
    DIA(const COO<valueType> &matrix);
    ~DIA();


    valueType get(size_t row, size_t col) const;
    DIA<valueType> & set(size_t row, size_t col, valueType val); // set for elements not equal 0
//    DIA<valueType> & remove(size_t row, size_t col);



    DIA<valueType> operator * (valueType const co) ;
    // SPmV
    std::vector<valueType> operator * (std::vector<valueType> v);
//    DIA<valueType> operator * (DIA<valueType> const &matrix) ;
//    DIA<valueType> operator + (DIA<valueType> const &matrix) ;

protected:
    valueType ** _values;
    std::vector<long> *_distance=nullptr;
    size_t _array_r, _array_c; // describe the 2d array to store the values

    CompressType _type = dia;

    void construct(size_t _array_l, size_t _pointer_l);
};



template <typename valueType>
DIA<valueType>::DIA(size_t n) :SparseMatrix<valueType>(n){
    this->_array_r = n;
    this->_array_c = 2*n;
    this->_distance = new std::vector<valueType>();
    this->_values = new int *[_array_r];
}

template <typename valueType>
DIA<valueType>::DIA(size_t row, size_t col) :SparseMatrix<valueType>(row,col){
    this->_array_r = row<col?row:col;
    this->_array_c = row<col?col*2:row*2;
    this->_distance = new std::vector<valueType>();
    this->_values = new int *[_array_c];
}

template <typename valueType>
void DIA<valueType>::construct(size_t _array_l, size_t _pointer_l) {
    this->_array_r = _array_l;
    this->_array_c = _pointer_l;
    this->_values = new valueType *[_pointer_l];
    for (size_t i =0; i<_pointer_l; i++){
        this->_values[i] = new valueType [_array_l];
    }
}

template <typename valueType>
DIA<valueType>::DIA(const DIA<valueType> &matrix) :SparseMatrix<valueType>(matrix){
    this->~DIA();
    this->construct(matrix._array_r,matrix._array_c);

    size_t _p = this->_array_c, _l=this->_array_r;
    for (size_t i =0; i<_p; i++)
        for (size_t j =0; j<_l; j++)
            this->_values[i][j] = matrix._values[i][j];
    this->_distance = new std::vector<long>(*matrix._distance);
}

template <typename valueType>
DIA<valueType>::DIA(const DNS<valueType> &matrix) :SparseMatrix<valueType>(matrix){
    size_t row = matrix.getColCount();
    size_t col = matrix.getRowCount();
    this->construct(row, col);

    for (size_t i = row; i > 0; i++){
        size_t tmp_r = row-1;
        size_t tmp_c = 0;

        valueType *tmp_values = new valueType[this->_array_r]();

        size_t c = 0;
        while (tmp_r<row && tmp_c<col){
            if (matrix._values[tmp_r][tmp_c]!=valueType()){
                tmp_values[tmp_r] = matrix._values[tmp_r][tmp_c];
                c++;
            }
            tmp_c++; tmp_r++;
        }

        if (c==0){
            delete [] tmp_values;
        } else{
            this->_distance->push_back(0 -(long)tmp_r);
            this->_values[this->_distance->size()-1] = tmp_values;
        }
    }

    for (size_t i = 1; i< col; i++){
        size_t tmp_r = 0;
        size_t tmp_c =i;

        valueType *tmp_values = new valueType[this->_array_r]();

        size_t c = 0;
        while (tmp_r<row && tmp_c<col){
            if (matrix._values[tmp_r][tmp_c]!=valueType()){
                tmp_values[tmp_r] = matrix._values[tmp_r][tmp_c];
                c++;
            }
            tmp_c++; tmp_r++;
        }

        if (c==0){
            delete [] tmp_values;
        } else{
            this->_distance->push_back((long)tmp_c);
            this->_values[this->_distance->size()-1] = tmp_values;
        }
    }
}

// col-row get the offset to the main diagonal then use row to get the real value
template <typename valueType>
valueType DIA<valueType>::get(size_t row, size_t col) const {
    long offset = (long)col - (long)row;
    long index=-1;
    for (size_t i =0; i<this->_distance->size(); i++){
        if (this->_distance->at(i) == offset)
            index = i;
    }
    if (index == -1){
        return valueType();
    } else {
        return this->_values[index][row];
    }
}

template <typename valueType>
DIA<valueType>& DIA<valueType>::set(size_t row, size_t col, valueType val) {
    long offset = (long)col - (long)row;
    long index=-1;
    for (size_t i =0; i<this->_distance->size(); i++){
        if (this->_distance->at(i) == offset)
            index = i;
    }

    if (index == -1){
        this->_distance->push_back(offset);
        valueType * p = new valueType[this->_array_r]();
        p[row] = val;
        this->_values[this->_distance->size()-1] = p;
    } else {
        this->_values[row][col] = val;
    }

    return *this;

}

template <typename valueType>
DIA<valueType> DIA<valueType>::operator*(valueType const co) {
    DIA<valueType> *RET = new DIA<valueType>(*this);
    size_t t = RET->_distance->size();
    for (size_t i =0; i<t;i++){
        for (size_t j = 0; j<this->_array_r;j++){
            RET->_values[i][j] *= co;
        }
    }
    return *RET;
}

template <typename valueType>
DIA<valueType>::DIA(const COO<valueType> &matrix) :SparseMatrix<valueType>(matrix){
    std::vector<valueType> &values = *(matrix._values);
    std::vector<size_t> &rowIndex = *(matrix._rowIndex);
    std::vector<size_t> &colIndex = *(matrix._colIndex);
    std::map<long, size_t > getIndex;

    size_t length = values.size();

    // add all distance to a set(unique)
    std::set<long> ns;
    for ( size_t i =0; i<length; i++){
        ns.insert(long(colIndex[i] - rowIndex[i]));
    }

    if(this->_distance == nullptr){
        this->_distance =new std::vector<long>;
    }
    // put the unique values into
    // and a map with diatance:index
    size_t rel_i = 0;
    for (auto dis:ns){
        this->_distance->push_back(dis);
//        std::cout<<"dis is: "<<dis<<" with index:" << rel_i<<std::endl;
        getIndex.insert(std::make_pair(dis,rel_i++));
    }

    this->_values = new valueType*[matrix.getRowCount()];
    this->_array_r = matrix.getRowCount();
    this->_array_c = ns.size();

    for (size_t i=0;i<matrix.getRowCount();i++){
        this->_values[i] = new valueType[ns.size()];
    }
//    std::cout<<"left filling"<<std::endl;

    for ( size_t i =0; i<length; i++){
        size_t c = colIndex[i];
        size_t r = rowIndex[i];

        size_t re_c = getIndex[long(c-r)];
        this->_values[r][re_c] = values[i];
    }

}

template <typename valueType>
std::vector<valueType> DIA<valueType>::operator*(std::vector<valueType> v) {
    if (this->getColCount() != v.size()){
//        return NULL;
    }

    size_t row = this->getRowCount();
    size_t col = this->_array_c;


    std::vector<valueType> RET(row,valueType(0));

    for (size_t i=0; i<row; i++){
        for (size_t j=0; j<col; j++){
            valueType val = this->_values[i][j];
            if (val != valueType(0)){
                long index = long(i + this->_distance->at(j));
                RET[i] += v[index] * this->_values[i][j];
            }
        }
    }

    return RET;
}

template <typename valueType>
DIA<valueType>::~DIA() {

    for (unsigned int i=0;i<this->_array_r;i++){
//        for (unsigned int j=0;i<this->_array_c;j++){
//            delete _values[i][j];
//        }
        delete [] _values[i];
    }
    delete [] this->_values;
    this->_values = nullptr;
    delete _distance;
    this->_distance = nullptr;
}


#endif //PLAYGROUND_DIA_H
