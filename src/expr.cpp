#include <string>
#include <iostream>
#include <memory>
#include <math.h>
#include "visitor.h"
#include "expr.h"
#include "data.h"

using namespace std;

Expr::Expr(ExprType type_): type(type_) {}
ExprType Expr::get_type() const {return type;}
Expr::~Expr() {}; 

// ===== BINARY =====
Binary::Binary(shared_ptr<Expr> left_, Token op_, shared_ptr<Expr> right_) : Expr(BINARY), left(left_), right(right_), op(op_) {}
Data Binary::accept(const ExprVisitor* visitor) const {
    return visitor->visit_binary(*(dynamic_cast<const Binary*>(this)));
}

// ===== LOGICAL =====
Logical::Logical(shared_ptr<Expr> left_, Token op_, shared_ptr<Expr> right_) : Expr(LOGICAL), left(left_), right(right_), op(op_) {}
Data Logical::accept(const ExprVisitor* visitor) const {
    return visitor->visit_logical(*(dynamic_cast<const Logical*>(this)));
}

// ===== UNARY =====
Unary::Unary(Token op_, shared_ptr<Expr> right_) : Expr(UNARY), right(right_), op(op_) {}
Data Unary::accept(const ExprVisitor* visitor) const {
    return visitor->visit_unary(*(dynamic_cast<const Unary*>(this)));
}

// ===== GROUPING =====
Grouping::Grouping(shared_ptr<Expr> expr_) : Expr(GROUPING), expr(expr_) {}
Data Grouping::accept(const ExprVisitor* visitor) const {
    return visitor->visit_grouping(*(dynamic_cast<const Grouping*>(this)));
}

// ===== LITERAL =====

Literal::Literal(double value) : Expr(LITERAL), data(value) {}
Literal::Literal(string value) : Expr(LITERAL), data(value.c_str()) {}
Literal::Literal(bool value) : Expr(LITERAL), data(value) {}
Literal::Literal() : Expr(LITERAL), data() {}
Data Literal::accept(const ExprVisitor* visitor) const {
    return visitor->visit_literal(*(dynamic_cast<const Literal*>(this)));
}

// ===== VARIABLE =====

Variable::Variable(Token id_) : Expr(VARIABLE), id(id_) {}
Data Variable::accept(const ExprVisitor* visitor) const {
    return visitor->visit_variable(*(dynamic_cast<const Variable*>(this)));
}
