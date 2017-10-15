//
// Created by zodiac on 30/04/17.
//

#ifndef CPPMETA_INTALU_H
#define CPPMETA_INTALU_H

class Expr_base{
public:
    virtual int eval(int x) const{}
};

template <class BinOp, class L, class R> class Expr:public Expr_base{
    const L &l_;
    const R &r_;
public:
    Expr(const L &l, const R &r):l_(l),r_(r){};
    int eval(int x) const{
        return BinOp::apply(l_,r_,x);
    }
};



template <> class Expr<int,int,int>:public Expr_base{
    const int value_;
public:
    Expr(const int value):value_(value){};
    Expr(Expr<int,int,int> &obj):value_(obj.getvalue()){};
    int eval(int x) const{
        return value_;
    }
    int getvalue() const{
        return this->value_;
    }
};
typedef Expr<int,int,int> Number;


template <> class Expr<char,char,char>:public Expr_base{
    const char name_;
public:
    Expr(const char name):name_(name){};
    Expr(Expr<char,char,char> &obj):name_(obj.getname()){};
    int eval(int x) const{
        return x;
    }
    char getname() const{
        return this->name_;
    }
};
typedef Expr<char,char,char> Variable;



// define the add and overload the operator +
class ADD{
public:
    static inline int apply(const Expr_base &a, const Expr_base &b, int x)
    { return a.eval(x) + b.eval(x); }
};
template <class L, class R> Expr<ADD, L, R> operator+(const L &l, const R &r){
    return Expr<ADD, L, R>(l,r);
};


// define the SUB and overload the operator -
class SUB{
public:
    static inline int apply(const Expr_base &a, const Expr_base &b, int x)
    { return a.eval(x) - b.eval(x); }
};
template <class L, class R> Expr<SUB, L, R> operator-(const L &l, const R &r){
    return Expr<SUB, L, R>(l,r);
};


// define the MUL and overload the operator *
class MUL{
public:
    static inline int apply(const Expr_base &a, const Expr_base &b, int x)
    { return a.eval(x) * b.eval(x); }
};
template <class L, class R> Expr<MUL, L, R> operator*(const L &l, const R &r){
    return Expr<MUL, L, R>(l,r);
};


// define the add and overload the operator /
class DIV{
public:
    static inline int apply(const Expr_base &a, const Expr_base &b, int x)
    { return a.eval(x) / b.eval(x); }
};
template <class L, class R> Expr<DIV, L, R> operator/(const L &l, const R &r){
    return Expr<DIV, L, R>(l,r);
};











#endif //CPPMETA_INTALU_H
