#ifndef EXPR_H_INCLUDED
#define EXPR_H_INCLUDED 1

#include <string>
#include "token.h"
#include "data.h"

class Expr {
    // Defines the base Expr class, from which all expression types inherit.
    public:
        // This function is overrided and is never called as is.
        virtual ~Expr(); // https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
};

class Binary : virtual public Expr {
    public:
        // A Binary expression takes two Expression operands and a operator.
        Binary(Expr* left_, Token op_, Expr* right_);
        // A destructor is necessary to delete the nested Expression nodes.
        ~Binary();
    private:
        Expr *left, *right;
        Token op;
};
class Unary : virtual public Expr {
    public:
        Unary(Token op_, Expr* right_);
        ~Unary();
    private:
        Token op;
        Expr *right;
};

class Grouping : virtual public Expr {
    public:
        Grouping(Expr* expr_);
        ~Grouping();
    private:
        Expr *expr;
};

/* "NumberLiteral", "StringLiteral", "BooleanLiteral" and "NilLiteral" defines Lua literal expressions, which consists of single explicit values.
Eg: 120, "abc", true, nil */

class NumberLiteral: virtual public Expr {
    public:
        NumberLiteral(double value);
        ~NumberLiteral();
    private:
        const Data data;
};
class StringLiteral: virtual public Expr {
    public:
        //StringLiteral(const char* value);
        StringLiteral(std::string value);
        ~StringLiteral();
    private:
        const Data data;
};
class BooleanLiteral: virtual public Expr {
    public:
        BooleanLiteral(bool value);
        ~BooleanLiteral();
    private:
        const Data data;
};
class NilLiteral: virtual public Expr {
    public:
        NilLiteral();
        ~NilLiteral();
    private:
        const Data data;
};


#endif
