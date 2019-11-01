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
LuaData* Binary::interpret() {
    LuaData *leftData = left->interpret();
    LuaData *rightData = right->interpret();

    // ==== Arithmetic operators:
    // Binary '-' expects two number operands and returns their subtraction. 
    if (op.type == "MINUS") return new LuaNumber(leftData->eval_number() - rightData->eval_number());
    // Binary '+' expects two number operands and returns their sum.
    else if (op.type == "PLUS") return new LuaNumber(leftData->eval_number() + rightData->eval_number());
    // Binary '*' expects two number operands and returns their multiplication
    else if (op.type == "STAR") return new LuaNumber(leftData->eval_number() * rightData->eval_number());
    // Binary '/' expects two number operands and returns their division.
    else if (op.type == "SLASH") return new LuaNumber(leftData->eval_number() / rightData->eval_number());
    // Binary '//' expects two number operands and returns their integer division.
    else if (op.type == "DOUBLE_SLASH") return new LuaNumber(floor(leftData->eval_number() / rightData->eval_number()) );
    // Binary '%' expects two number operands and returns their division remainder (modulo).
    //else if (op.type == "PERCENT") return new LuaNumber(leftData->eval_number() % rightData->eval_number());
    // Binary '^' expects two number operands and returns their exponentiation. PRECISO ADICIONAR O PARSE DISSO.
    //else if (op.type == "CIRCUMFLEX") return new LuaNumber(pow(leftData->eval_number(), rightData->eval_number()));

    // ===== Relational operators:
    // Binary '==' returns equality between two values. Equality in Lua is explained in helper function "isEqual".
    else if (op.type == "DOUBLE_EQUAL") return new LuaBoolean(isEqual(leftData, rightData));
    // Binary '~=' returns opposite of equality between two values.
    else if (op.type == "NOT_EQUAL") return new LuaBoolean(isNotEqual(leftData, rightData));
    // Falta implementar os comparadores.
    cout << "Se nao for comparador, nao era pra tar aqui!" << endl << "op.type = " << op.type << endl;
    return new LuaBoolean(luaTrue);
}
LuaSpecialValue Binary::isEqual(LuaData *left, LuaData *right) {
    // Always returns false if operands are of different types. Otherwise, compares operands by value.
    LuaDataType leftType = left->get_type();
    if (leftType != right->get_type())
        return luaFalse;
    switch (leftType) {
        case luaNumber:
            return (left->eval_number() == right->eval_number()? luaTrue : luaFalse);
        break;
        case luaString:
            return (left->eval_string() == right->eval_string()? luaTrue : luaFalse);
        break;
        case luaBoolean:
            return (left->eval_boolean() == right->eval_boolean()? luaTrue : luaFalse);
        break;
        case luaNilType:
            return luaTrue;    
    }
    cout << "Should not be here! sad2109dsç" << endl;
    return luaTrue;
}
LuaSpecialValue Binary::isNotEqual(LuaData *left, LuaData *right) {
    // Returns the Lua boolean opposite of "isEqual".
    return (isEqual(left, right) == luaTrue? luaFalse : luaTrue);
}

// ==========

// ===== UNARY =====
Unary::Unary(Token op_, Expr* right_) : op(op_), right(right_) {}
Unary::~Unary() {if (right) delete (right);}
LuaData* Unary::interpret() {
    LuaData *rightData = right->interpret();
    // Unary '-' expects a number as operand and returns its additive inverse.
    if (op.type == "MINUS") return new LuaNumber(-(rightData->eval_number()) );
    // Unary '#' expects a string as operand and returns its length.
    else if (op.type == "HASH") return new LuaNumber(rightData->eval_string().size());
    // Unary 'not' returns true if operand is "falsy" (opposite of "truthy"), and returns false otherwise. "Falsy" values in Lua are "false" and "nil". 
    else if (op.type == "NOT") return rightData->is_truthy();

    // TODO remover isso aqui
    cout << "Should not be here! GDSFA";
    exit (1);
}
// ==========

// ===== GROUPING =====
Grouping::Grouping(Expr* expr_) : expr(expr_) {}
Grouping::~Grouping() {if (expr) delete (expr);}
LuaData* Grouping::interpret() {
    return expr->interpret();
}
// ==========


// ===== NUMBER LITERAL =====
NumberLiteral::NumberLiteral(double value) {data = new LuaNumber(value);}
NumberLiteral::~NumberLiteral() {if (data) delete(data);}
LuaData* NumberLiteral::interpret() {
    return data;
}
// ==========


// ===== STRING LITERAL =====
StringLiteral::StringLiteral(string value) {data = new LuaString(value);}
StringLiteral::~StringLiteral() {if (data) delete(data);}
LuaData* StringLiteral::interpret() {
    return data;
}
// ==========

// ===== BOOLEAN LITERAL =====
BooleanLiteral::BooleanLiteral(LuaSpecialValue value) {data = new LuaBoolean(value);}
BooleanLiteral::~BooleanLiteral() {if (data) delete(data);}
LuaData* BooleanLiteral::interpret()  {
    return data;
}
// ==========

// ===== NIL LITERAL =====
NilLiteral::NilLiteral() {data = new LuaNil();}
NilLiteral::~NilLiteral() {if (data) delete(data);}
LuaData* NilLiteral::interpret() {
    return data;
}
// ==========
