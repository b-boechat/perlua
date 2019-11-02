#include <string>
#include <iostream>
#include <math.h>
#include "expr.h"
#include "data.h"

using namespace std;

Expr::Expr(ExprType type_): type(type_) {}
ExprType Expr::get_type() const {return type;}
Expr::~Expr() {}; 

// ===== BINARY =====
Binary::Binary(Expr* left_, Token op_, Expr* right_) : Expr(BINARY), left(left_), right(right_), op(op_) {}
Binary::~Binary() { if (left) delete (left); if (right) delete (right); }

// ===== UNARY =====
Unary::Unary(Token op_, Expr* right_) : Expr(UNARY), right(right_), op(op_) {}
Unary::~Unary() {if (right) delete (right);}

// ===== GROUPING =====
Grouping::Grouping(Expr* expr_) : Expr(GROUPING), expr(expr_) {}
Grouping::~Grouping() {if (expr) delete (expr);}

// ===== LITERALS =====

Literal::Literal(double value) : Expr(LITERAL), data(value) {}
Literal::Literal(string value) : Expr(LITERAL), data(value.c_str()) {}
Literal::Literal(bool value) : Expr(LITERAL), data(value) {}
Literal::Literal() : Expr(LITERAL), data() {}
