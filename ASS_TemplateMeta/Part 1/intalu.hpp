//
// Created by zodiac on 10/05/17.
//

#ifndef CPPMETA_INTALU_H
#define CPPMETA_INTALU_H

// LIT<1>, LIT<2>
template <int N> class LIT{
public:
    static inline int eval(int x) {
        return N;
    }
};

// Variable
class VAR{
public:
    static inline int eval(int x) {
        return x;
    }
};

// ADD<L,R> L+R
template <class L,class R> class ADD{
public:
    static inline int eval(int x) {
        return L::eval(x) + R::eval(x);
    }
};

// SUB<L,R> L-R
template <class L, class R> class SUB{
public:
    static inline int eval(int x) {
        return L::eval(x) - R::eval(x);
    }
};

// MUL<L,R> L*R
template <class L,class R> class MUL{
public:
    static inline int eval(int x) {
        return L::eval(x) * R::eval(x);
    }
};

// DIV<L,R> L/R
template <class L,class R> class DIV{
public:
    static inline int eval(int x) {
        return L::eval(x) / R::eval(x);
        }
};

#endif //CPPMETA_INTALU_H
