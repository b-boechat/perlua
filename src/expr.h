#ifndef EXPR_H_INCLUDED
#define EXPR_H_INCLUDED 1

/*
 * This header defines the Expresson AST nodes, which are created by the parser and traversed by the evaluator. 
 */



#include <string>
#include "token.h"
#include "visitor.h"
#include "data.h"

// Tirar isso depois, é só pra debugging. TODO
enum ExprType {BINARY, LOGICAL, UNARY, GROUPING, LITERAL, VARIABLE};


class Expr {
    // Defines the base Expr class, from which all expression types inherit.
    public:
        Expr(ExprType type_);
        ExprType get_type() const;
        virtual ~Expr(); 
        virtual Data accept(const ExprVisitor* visitor) const = 0;
        // Saves expression type, useful to the evaluator.
        ExprType type;
};

class Binary : virtual public Expr {
    public:
        // A Binary expression takes two Expression operands and a operator.
        Binary(Expr* left_, Token op_, Expr* right_);
        // A destructor is necessary to delete the nested Expression nodes.
        ~Binary();
        Data accept(const ExprVisitor* visitor) const override;
        const Expr *left, *right;
        const Token op;
};

class Logical : virtual public Expr {
    public:
        Logical(Expr* left_, Token op_, Expr* right_);
        ~Logical();
        Data accept(const ExprVisitor* visitor) const override;
        const Expr *left, *right;
        const Token op;
};

class Unary : virtual public Expr {
    public:
        Unary(Token op_, Expr* right_);
        ~Unary();
        Data accept(const ExprVisitor* visitor) const override;
        const Expr *right;
        const Token op;
};

class Grouping : virtual public Expr {
    public:
        Grouping(Expr* expr_);
        ~Grouping();
        Data accept(const ExprVisitor* visitor) const override;
        const Expr *expr;
};


class Literal : virtual public Expr {
    public:
        Literal(double value);
        Literal(std::string value);
        Literal(bool value);
        Literal();
        Data accept(const ExprVisitor* visitor) const override;
        const Data data;
};

class Variable : virtual public Expr {
    public: 
        // TODO talvez não precise de token, e sim de string mesmo.
        Variable(Token id_);
        Data accept(const ExprVisitor* visitor) const override;
        const Token id; 
};

#endif
