//
// Created by zoELLc on 08/08/17.
//

#ifndef PLAYGROUND_ELL_H
#define PLAYGROUND_ELL_H

#include "SparseMatrix.hpp"
#include "DNS.hpp"
#include "COO.hpp"

template <typename valueType>
class ELL :public SparseMatrix<valueType>{
public:
    ELL(const ELL<valueType> &matrix);
    ELL(const COO<valueType> &matrix);
//    ELL(const DNS<T> &matrix);
    ~ELL();


//    T get(unsigned long row, unsigned long col) const;
//    ELL<T> & set(unsigned long row, unsigned long col, T val); // set for elements not equal 0
//    ELL<T> & remove(unsigned long row, unsigned long col);


    ELL<valueType> operator * (valueType const co) ;
    // SPmV
    std::vector<valueType> operator * (std::vector<valueType> v);


protected:
    valueType ** _values;
    size_t ** _colIndex;

    size_t _array_r, _array_c;
    CompressType _type = ell;

};

template <typename valueType>
ELL<valueType>::ELL(const ELL<valueType> &matrix) :SparseMatrix<valueType>(matrix){
    this->~ELL();

    this->_array_c = matrix._array_c;
    this->_array_r = matrix._array_r;

    this->_values = new valueType*[_array_r];
    for(size_t i =0; i<_array_r; i++){
        this->_values[i] = new valueType[_array_c];
    }
}

template <typename valueType>
ELL<valueType>::ELL(const COO<valueType> &matrix) :SparseMatrix<valueType>(matrix){

    std::map<size_t , int> nz;
    this->_array_r = matrix.getRowCount();
    size_t  row = this->_array_r;
    size_t *count = new size_t[row]();


    std::vector<valueType> &values = *(matrix._values);
    std::vector<size_t> &rowIndex = *(matrix._rowIndex);
    std::vector<size_t> &colIndex = *(matrix._colIndex);
    size_t length = values.size();
//    std::cout<<length<<std::endl;

    // count how many elements in each row and find the largest one
    for(auto i:rowIndex){count[i] += 1;}
    size_t max = count[0];
    for(size_t i =1; i<row;i++){
        max>count[i] ? max : max = count[i];
    }

    this->_array_c = max;

    this->_values = new valueType* [row];
    this->_colIndex = new size_t* [row];

    for(size_t i=0; i<row; i++){
        this->_values[i] = new valueType[max]();
        this->_colIndex[i] = new size_t[max]();
    }

    delete[] count;
    count = new size_t[row]();

//    std::cout<<"aaa"<<count[458]<<std::endl;

    for(size_t i=0; i<length; i++){
//        std::cout<<i<<std::endl;
        size_t tr = rowIndex[i];
        size_t tc = colIndex[i];
        valueType val = values[i];

        size_t tmp = count[tr];
        count[tr] += 1;
//        std::cout<<"tr "<<tr<<" tc "<<tc<<" p "<<tmp<<std::endl;
        this->_colIndex[tr][tmp] = tc;
        this->_values[tr][tmp] = val;

    }


}

template <typename valueType>
ELL<valueType> ELL<valueType>::operator*(valueType const co) {
    size_t &array_r = this->_array_r;
    size_t &array_c = this->_array_c;

    for(size_t i=0; i<array_r; i++){

        for(size_t j=0; j<array_c; j++) {
            if(this->_values[i][j] == valueType(0)) break;
            this->_values[i][j] *= co;
        }

    }
    return *this;
}

template <typename valueType>
std::vector<valueType> ELL<valueType>::operator*(std::vector<valueType> v) {


    if (this->getColCount() != v.size()){
//        return NULL;
    }

    size_t &row = this->_rowCount;
    size_t &col = this->_colCount;

    std::vector<valueType> RET(row,valueType(0));

    size_t &array_r = this->_array_r;
    size_t &array_c = this->_array_c;


//    std::cout<<array_r<<"   "<<row<<"   "<<array_c<<"   v[931]:"<<RET[931]<<std::endl;

    for(size_t i=0; i<row; i++){

//        std::cout<<i<<std::endl;
        valueType tmp_val = valueType(0);
        for(size_t j=0; j<array_c; j++){
//            std::cout<<"before checking 0"<<std::endl;
            if(this->_values[i][j] == valueType(0)) break;
            size_t c = this->_colIndex[i][j];
            tmp_val += this->_values[i][j] * v[c];
//            std::cout << "the value will become"<<tmp_val<<std::endl;
//            std::cout << j<<"  222  "<<RET[i]<<std::endl;
//            RET[i] += val;
//            RET[i] += this->_values[i][j]*v[c];
        }
//        std::cout<<"set the "<<i<<"th row result"<<std::endl;
        RET[i] = tmp_val;
    }

    return RET;

}

template <typename valueType>
ELL<valueType>::~ELL() {
//    std::cout<<"start loop"<<std::endl;
    for (unsigned int i=0;i<this->_array_r;i++){
//        for (unsigned int j=0;i<this->_array_c;j++){
//            delete _values[i][j];
//            delete _colIndex[i][j];
//        }
//        std::cout<<"the "<<i<<"th line"<<std::endl;

        delete [] this->_values[i];
        delete [] this->_colIndex[i];
    }
    delete [] this->_values;
    delete [] this->_colIndex;
    this->_values = nullptr;
    this->_colIndex = nullptr;
}

#endif //PLAYGROUND_ELL_H
