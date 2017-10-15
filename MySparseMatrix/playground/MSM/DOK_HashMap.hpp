//
// Created by zodiac on 07/08/17.
//

#ifndef PLAYGROUND_DOK_HASHMAP_H
#define PLAYGROUND_DOK_HASHMAP_H

#include <map>
#include <unordered_map>
#include "SparseMatrix.hpp"
#include <tuple>
#include "DNS.hpp"
#include "COO.hpp"

//from https://stackoverflow.com/questions/32685540/c-unordered-map-with-pair-as-key-not-compiling

// Only for pairs of std::hash-able types for simplicity.
// You can of course template this struct to allow other hash functions
struct tuple_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::tuple<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(std::get<0>(p));
        auto h2 = std::hash<T2>{}(std::get<1>(p));

        // Mainly for demonstration purposes, i.e. works but is overly simple
        // In the real world, use sth. like boost.hash_combine
        return h1 ^ h2;
    }
};

template <typename valueType>
class DOK_HashMap :public SparseMatrix<valueType>{
public:
    typedef std::tuple<size_t, size_t> coordinate;
    typedef std::unordered_map<coordinate, valueType, tuple_hash> TupleHashMap;
    DOK_HashMap(size_t n);
    DOK_HashMap(size_t row, size_t col);
    DOK_HashMap(const DOK_HashMap<valueType> &matrix);
    DOK_HashMap(const DNS<valueType> &matrix);
    DOK_HashMap(const COO<valueType> &matrix);
    ~DOK_HashMap();


    valueType get(size_t row, size_t col) const;
    DOK_HashMap<valueType> & set(size_t row, size_t col, valueType val); // set for elements not equal 0
    DOK_HashMap<valueType> & remove(size_t row, size_t col);

    valueType get(coordinate key) const;
    DOK_HashMap<valueType> & set(coordinate key, valueType val);
    DOK_HashMap<valueType> & remove(coordinate key);

    DOK_HashMap<valueType> operator * (valueType const co) ;
    // SPmV
    std::vector<valueType> operator * (std::vector<valueType> v);
//    DOK_HashMap<valueType> operator * (DOK_HashMap<valueType> const &matrix) ;
    DOK_HashMap<valueType> operator + (DOK_HashMap<valueType> const &matrix) ;

protected:
    TupleHashMap* _values= nullptr;
    CompressType _type=dok;

};


template <typename valueType>
DOK_HashMap<valueType>::DOK_HashMap(size_t n) : SparseMatrix<valueType>(n){}

template <typename valueType>
DOK_HashMap<valueType>::DOK_HashMap(size_t row, size_t col) :SparseMatrix<valueType>(row,col){}

template <typename valueType>
DOK_HashMap<valueType>::DOK_HashMap(const DOK_HashMap<valueType> &matrix) :SparseMatrix<valueType>(matrix){
    this->_values = new TupleHashMap (*matrix._values);
}

template <typename valueType>
DOK_HashMap<valueType>::DOK_HashMap(const DNS<valueType> &matrix) :SparseMatrix<valueType>(matrix){
    this->~DOK_HashMap();


    TupleHashMap  &values = *(this->_values);

    valueType** nvalues = matrix._values;
    size_t rc = matrix.getRowCount();
    size_t cc = matrix.getColCount();

    for (size_t i = 0; i < rc; i++){
        for (size_t j = 0; j < cc; j++){
            if (nvalues[i][j] == valueType()){
                coordinate key = std::make_tuple(i,j);
                values[key] = nvalues[i][j];
            }
        }
    }
}

template <typename valueType>
DOK_HashMap<valueType>::~DOK_HashMap() {
    delete this->_values;
}

template <typename valueType>
DOK_HashMap<valueType>& DOK_HashMap<valueType>::set(size_t row, size_t col, valueType val) {
    return this->set(std::make_tuple(row,col), val);
}

template <typename valueType>
valueType DOK_HashMap<valueType>::get(size_t row, size_t col) const {
    return this->get(std::make_tuple(row,col));
}

template <typename valueType>
valueType DOK_HashMap<valueType>::get(coordinate key) const {
    TupleHashMap & value = *(this->_values);
    return value[key];

}

template <typename valueType>
DOK_HashMap<valueType>& DOK_HashMap<valueType>::set(coordinate key, valueType val) {
    TupleHashMap & value = *(this->_values);
    value[key] = val;
    return *this;
}

template <typename valueType>
DOK_HashMap<valueType>& DOK_HashMap<valueType>::remove(size_t row, size_t col) {
    return this->remove(std::make_tuple(row,col));
}

template <typename valueType>
DOK_HashMap<valueType>& DOK_HashMap<valueType>::remove(coordinate key) {
    TupleHashMap & value = *(this->_values);
    auto it = value.find(key);
    if (value.count(key)){
        value.erase(it);
    }

    return *this;
}


template <typename valueType>
DOK_HashMap<valueType> DOK_HashMap<valueType>::operator *(valueType const co) {
    DOK_HashMap<valueType> *RET = new DOK_HashMap<valueType>(*this);

    for(auto it : *(RET->_values))
        it->second *= co;

    return *RET;
}

template <typename valueType>
DOK_HashMap<valueType> DOK_HashMap<valueType>::operator +(DOK_HashMap<valueType> const &matrix) {
    DOK_HashMap<valueType> &matrix_1 = *this;
    DOK_HashMap<valueType> *n = new DOK_HashMap<valueType>(matrix);
    DOK_HashMap<valueType> &matrix_2 = *n;
    if (matrix_1.getColCount() != matrix_2.getColCount() || matrix_1.getRowCount() != matrix_2.getRowCount()){
        // throw error
    }

    for (auto it : *(this->_values)){
        coordinate key = it->first;
        valueType val_1 = it->second;
        valueType val_2 = matrix_2._values[key];
        valueType val = val_1 + val_2;

        if (val-0 < _my0 && val_2 - 0 > _my0){
            matrix_2.remove(key);
        } else {

            matrix_2.set(key, val);
        }
    }

    return matrix_2;

}

template <typename valueType>
std::vector<valueType> DOK_HashMap<valueType>::operator*(std::vector<valueType> v) {

    if (this->getColCount() != v.size()){
        //return NULL;
    }

    size_t row = this->getRowCount();

    std::vector<valueType> RET(row,valueType(0));
    TupleHashMap &values = *(this->_values);
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
DOK_HashMap<valueType>::DOK_HashMap(const COO<valueType> &matrix) :SparseMatrix<valueType>(matrix){

//    this->~DOK_HashMap();


    std::vector<valueType> &values = *(matrix._values);
    std::vector<size_t> &rowIndex = *(matrix._rowIndex);
    std::vector<size_t> &colIndex = *(matrix._colIndex);
    size_t length = values.size();

    if (this->_values == nullptr){
        this->_values = new TupleHashMap;
    }

    for(size_t i=0; i<length; i++){
        size_t tr = rowIndex[i];
        size_t tc = colIndex[i];
        valueType val = values[i];

        this->set(tr,tc,val);
    }
}

#endif //PLAYGROUND_DOK_HASHMAP_H
