//
// Created by zodiac on 18/08/17.
//

#ifndef PLAYGROUND_METRIC_H
#define PLAYGROUND_METRIC_H

#include "MSM/SparseMatrix.hpp"
#include "MSM/COO.hpp"
#include "MSM/CSC.hpp"
#include "MSM/CSR.hpp"
#include "MSM/DOK.hpp"
#include "MSM/DIA.hpp"
#include "MSM/DNS.hpp"
#include "MSM/ELL.hpp"
#include "MSM/DOK_HashMap.hpp"
#include <fstream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <boost/timer/timer.hpp>
#include <numeric>      // std::inner_product
#include <cmath>
#include <fstream>

template <typename valueType>
class Metric {
public:
    COO<valueType>& FromFile(const std::string &filePath, std::ofstream &o);
    // this will write file

    //std::tuple<time_t ,time_t >& RunTest(const std::string &filePath);
    void test(std::string &filePath, std::string &PathString);
    void SingleTest(SparseMatrix<valueType> &matrix, std::ofstream &o);
    valueType norm_2(std::vector<valueType> &v);


protected:
    COO<valueType>* _matrix_read;
    std::vector<valueType>* _starting_vector;
};


// inspired by http://www.cplusplus.com/forum/general/65804/
template <typename valueType>
COO<valueType>& Metric<valueType>::FromFile(const std::string &filePath, std::ofstream &o) {
    std::ifstream fin(filePath);
    int M, N, L;
    while (fin.peek() == '%') fin.ignore(2048, '\n');
    fin >> M >> N >> L;
    std::cout<<"rows: "<<M<<" cols: "<<N<<" nnz: "<<L<< std::endl;
    o<<"rows: "<<M<<" cols:"<<N<<" nnz:"<<L<< '\n';

    this->_matrix_read = new COO<valueType>(M,N);
    COO<valueType>& matrix_read = *(this->_matrix_read);

    std::cout<<"start reading  file"<<std::endl;
    for (size_t l = 0; l < L; l++)
    {
        size_t m, n;
        valueType data;
        fin >> m >> n >> data;
        matrix_read._rowIndex->push_back(m-1);
        matrix_read._colIndex->push_back(n-1);
        matrix_read._values->push_back(data);
    }
    fin.close();
    std::cout<<"file closed"<<matrix_read._colIndex->size()<<std::endl;
    return matrix_read;
}

template <typename valueType>
void Metric<valueType>::test(std::string &filePath, std::string &PathString) {

    std::string s(filePath.c_str());
    s.append(".output");

    std::ofstream myFile;
    myFile.open(s);
    myFile<<"FileName: "<<filePath<<"\nPath in UFget: "<<PathString<<'\n';

    this->FromFile(filePath,myFile);

    COO<valueType> &m_coo = *(this->_matrix_read);
    std::cout<<"Created COO"<<std::endl;
    myFile<<"Created COO"<<'\n';
    SingleTest(m_coo, myFile);
    std::cout<<"finished COO rows: "<<m_coo.getColCount()<<" cols: "<<m_coo.getRowCount()<<" nnz: "<<_matrix_read->_values->size()<<std::endl;
    myFile<<"finished COO rows: "<<m_coo.getColCount()<<" cols: "<<m_coo.getRowCount()<<" nnz: "<<_matrix_read->_values->size()<<'\n';


    DOK<valueType> m_dok( m_coo);
    std::cout<<"Converted DOK"<<std::endl;
    myFile<<"Converted DOK"<<'\n';
    SingleTest(m_dok, myFile);
    std::cout<<"finished DOK"<<std::endl;
    myFile<<"finished DOK"<<'\n';


    ELL<valueType> m_ell( m_coo);
    std::cout<<"Converted ELL"<<std::endl;
    myFile<<"Converted ELL"<<'\n';
    SingleTest(m_ell, myFile);
    std::cout<<"finished ELL"<<std::endl;
    myFile<<"finished ELL"<<'\n';


    DIA<valueType> m_dia( m_coo);
    std::cout<<"Converted DIA"<<std::endl;
    myFile<<"Converted DIA"<<'\n';
    SingleTest(m_dia, myFile);
    std::cout<<"finished DIA"<<std::endl;
    myFile<<"finished DIA"<<'\n';


    DNS<valueType> m_dns( m_coo);
    std::cout<<"Converted DNS"<<std::endl;
    myFile<<"Converted DNS"<<'\n';
    SingleTest(m_dns, myFile);
    std::cout<<"finished DNS"<<std::endl;
    myFile<<"finished DNS"<<'\n';


    CSR<valueType> m_csr( m_dns);
    std::cout<<"Converted CSR"<<std::endl;
    myFile<<"Converted CSR"<<'\n';
    SingleTest(m_csr, myFile);
    std::cout<<"finished CSR"<<std::endl;
    myFile<<"finished CSR"<<'\n';


    CSC<valueType> m_csc( m_dns);
    std::cout<<"Converted CSC"<<std::endl;
    myFile<<"Converted CSC"<<'\n';
    SingleTest(m_csc, myFile);
    std::cout<<"finished CSC"<<std::endl;
    myFile<<"finished CSC"<<'\n';


    DOK_HashMap<valueType> m_dokH( m_coo);
    std::cout<<"Converted DOK_HashMap"<<std::endl;
    myFile<<"Converted DOK_HashMap"<<'\n';
    SingleTest(m_dokH, myFile);
    std::cout<<"finished DOK_HashMap"<<std::endl;
    myFile<<"finished DOK_HashMap"<<'\n';

}

template <typename valueType>
void Metric<valueType>::SingleTest(SparseMatrix<valueType> &matrix, std::ofstream &o) {
    size_t size = matrix.getColCount();
    std::vector<valueType> v(size,1.0);
    boost::timer::auto_cpu_timer t(o);

    for(size_t i =0;i<300;i++) {
//        std::cout<<i<<std::endl;
        v = matrix * v;
        auto n = norm_2(v);
//        std::cout<<"the norm is: "<<n<<std::endl;
//        std::cout<<v[0]<<std::endl;
        for (auto &u:v) u /= n;
//        std::cout<<v[0]<<std::endl;
    }

}

template <typename valueType>
valueType Metric<valueType>::norm_2(std::vector<valueType> &v) {
    auto acc = std::inner_product(v.begin(),v.end(),v.begin(),valueType());
    return sqrt(acc);
}


#endif //PLAYGROUND_METRIC_H
