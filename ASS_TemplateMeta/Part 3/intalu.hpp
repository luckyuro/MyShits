//
// Created by zodiac on 10/05/17.
//

#ifndef CPPMETA_INTALU_H
#define CPPMETA_INTALU_H

#include <exception>
#include <sstream>

int VAR_LB, VAR_UB;

// LIT<1>,LIT<2> ...
template <int N> class LIT{
public:
    static inline int eval(int x[]) {
        return N;
    }
    enum{lb = N, ub = N};
};

// BOUND<L,U>
template <int L, int U> class BOUND{
public:
    enum {lb = L, ub = U};
};

class outOfBoundException: public std::exception{
    std::string s_;
public:
    outOfBoundException(std::string ss):s_(ss){}
    virtual const char* what() const throw(){
        return this->s_.c_str();
    }
};

// COMP<1,2,3,4> min=1, max=4
template <int A, int B, int C, int D> class COMP{
public:
    enum{min=(A<B)?((A<C)?(A<D?A:D):(C<D?C:D)):((B<C)?(B<D?B:D):(C<D?C:D)), max = (A>B)?((A>C)?(A>D?A:D):(C>D?C:D)):((B>C)?(B>D?B:D):(C>D?C:D))};
};


// VAR<1,'X',BOUND<L<U>> the first variable X L<=X<=U
template <int N, char M, class B>  class VAR{
public:
    static inline int eval(int list[]) {
        int value = list[N-1];
        if (value<lb || value>ub){
            std::stringstream errMsg;
            errMsg <<"the "<< N <<"st/th variable " << M <<" out of bound, it should between "<<lb <<" and "<<ub;
            throw outOfBoundException(errMsg.str());
        }
        return value;
    }
    enum {lb = B::lb, ub = B::ub};
};

// ADD<L,R> L+R
template <class L,class R> class ADD{
public:
    static inline int eval(int x[]) {
        return L::eval(x) + R::eval(x);
    }
    enum{lb = L::lb + R::lb, ub = L::ub + R::ub};
};

// SUB<L,R> L-R
template<class L, class R> class SUB{
public:
    static inline int eval(int x[]) {
        return L::eval(x) - R::eval(x);
    }
    enum{lb = L::lb - R::ub, ub = L::ub - R::lb};
};

// MUL<L,R> L*R
template<class L,class R> class MUL{
public:
    static inline int eval(int x[]) {
        return L::eval(x) * R::eval(x);
    }
    enum{lb = COMP<L::lb*R::lb,L::lb*R::ub,L::ub*R::lb,L::ub*R::ub>::min, ub = COMP<L::lb*R::lb,L::lb*R::ub,L::ub*R::lb,L::ub*R::ub>::max};
};

// DIV<L,R> L/R
template<class L,class R> class DIV{
public:
    static inline int eval(int x[]) {
        return L::eval(x) / R::eval(x);
        }
    enum{lb = COMP<L::lb/R::lb,L::lb/R::ub,L::ub/R::lb,L::ub/R::ub>::min, ub = COMP<L::lb/R::lb,L::lb/R::ub,L::ub/R::lb,L::ub/R::ub>::max};
};


#endif //CPPMETA_INTALU_H
