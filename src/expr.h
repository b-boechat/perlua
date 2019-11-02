#ifndef EXPR_H_INCLUDED
#define EXPR_H_INCLUDED 1

#include <string>
#include "token.h"
#include "data.h"

enum ExprType {BINARY, UNARY, GROUPING, LITERAL};


class Expr {
    // Defines the base Expr class, from which all expression types inherit.
    public:
        Expr(ExprType type_);
        ExprType get_type() const;
        virtual ~Expr(); // https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
        // Saves expression type, useful to the evaluator.
        ExprType type;
};

class Binary : virtual public Expr {
    public:
        // A Binary expression takes two Expression operands and a operator.
        Binary(Expr* left_, Token op_, Expr* right_);
        // A destructor is necessary to delete the nested Expression nodes.
        ~Binary();
        const Expr *left, *right;
        const Token op;
};
class Unary : virtual public Expr {
    public:
        Unary(Token op_, Expr* right_);
        ~Unary();
        const Expr *right;
        const Token op;
};

class Grouping : virtual public Expr {
    public:
        Grouping(Expr* expr_);
        ~Grouping();
        const Expr *expr;
};


class Literal : virtual public Expr {
    public:
        Literal(double value);
        Literal(std::string value);
        Literal(bool value);
        Literal();
        const Data data;
};

#endif
