#ifndef EXPR_H_INCLUDED
#define EXPR_H_INCLUDED 1

/*
 * This header defines the Expresson AST nodes, which are created by the parser and traversed by the evaluator. 
 */



#include <string>
#include <memory>
#include "token.h"
#include "visitor.h"
#include "data.h"

class Expr {
    // Defines the base Expr class, from which all expression types inherit.
    public:
        virtual ~Expr(); 
        virtual Data accept(const ExprVisitor* visitor) const = 0;
};

class Binary : virtual public Expr {
    public:
        // A Binary expression takes two Expression operands and a operator.
        Binary(std::shared_ptr<Expr> left_, Token op_, std::shared_ptr<Expr> right_);
        Data accept(const ExprVisitor* visitor) const override;
        const std::shared_ptr<Expr> left, right;
        const Token op;
};

class Logical : virtual public Expr {
    public:
        Logical(std::shared_ptr<Expr> left_, Token op_, std::shared_ptr<Expr> right_);
        Data accept(const ExprVisitor* visitor) const override;
        const std::shared_ptr<Expr> left, right;
        const Token op;
};

class Unary : virtual public Expr {
    public:
        Unary(Token op_, std::shared_ptr<Expr> right_);
        Data accept(const ExprVisitor* visitor) const override;
        const std::shared_ptr<Expr> right;
        const Token op;
};

class Grouping : virtual public Expr {
    public:
        Grouping(std::shared_ptr<Expr> expr_);
        Data accept(const ExprVisitor* visitor) const override;
        const std::shared_ptr<Expr> expr;
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
        Variable(Token id_);
        Data accept(const ExprVisitor* visitor) const override;
        const Token id; 
};

#endif
