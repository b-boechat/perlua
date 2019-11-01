#include <string>
#include <iostream>
#include <math.h>
#include "expr.h"
#include "data.h"

using namespace std;

Expr::~Expr() {}; 

// ===== BINARY =====
Binary::Binary(Expr* left_, Token op_, Expr* right_) : left(left_), right(right_), op(op_) {}
Binary::~Binary() { if (left) delete (left); if (right) delete (right); }

// ===== UNARY =====
Unary::Unary(Token op_, Expr* right_) : op(op_), right(right_) {}
Unary::~Unary() {if (right) delete (right);}

// ===== GROUPING =====
Grouping::Grouping(Expr* expr_) : expr(expr_) {}
Grouping::~Grouping() {if (expr) delete (expr);}

// ===== LITERALS =====

NumberLiteral::NumberLiteral(double value) : data(value) {}
//StringLiteral::StringLiteral(const char* value) : data(value) {}
StringLiteral::StringLiteral(string value) : data(value.c_str()) {}
BooleanLiteral::BooleanLiteral(bool value) : data(value) {}
NilLiteral::NilLiteral() : data() {}
