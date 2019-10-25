#ifndef EXPR_H_INCLUDED
#define EXPR_H_INCLUDED 1

#include <string>
#include "token.h"
#include "data.h"

class Expr {
    // Defines the base Expr class, from which all expression types inherit.
    public:
        // This function is overrided and is never called as is.
        virtual LuaData* interpret() = 0;
        virtual ~Expr(); // https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
};

class Binary : virtual public Expr {
    public:
        // A Binary expression takes two Expression operands and a operator.
        Binary(Expr* left_, Token op_, Expr* right_);
        // A destructor is necessary to delete the nested Expression nodes.
        ~Binary();
        LuaData* interpret() override;
    private:
        // Always returns false if operands are of different types. Otherwise, compares operands by value.
        LuaSpecialValue isEqual(LuaData *left, LuaData *right);
        // Returns the Lua boolean opposite of "isEqual".
        LuaSpecialValue isNotEqual(LuaData *left, LuaData *right);
        Expr *left, *right;
        Token op;
};
class Unary : virtual public Expr {
    public:
        Unary(Token op_, Expr* right_);
        ~Unary();
        LuaData* interpret() override;
    private:
        Token op;
        Expr *right;
};

class Grouping : virtual public Expr {
    public:
        Grouping(Expr* expr_);
        ~Grouping();
        LuaData* interpret() override;
    private:
        Expr *expr;
};

/* "NumberLiteral", "StringLiteral", "BooleanLiteral" and "NilLiteral" defines Lua literal expressions, which consists of single explicit values.
Eg: 120, "abc", true, nil */

class NumberLiteral: virtual public Expr {
    public:
        NumberLiteral(double value);
        ~NumberLiteral();
        LuaData* interpret() override;
    private:
        LuaNumber* data;
};
class StringLiteral: virtual public Expr {
    public:
        StringLiteral(std::string value);
        ~StringLiteral();
        LuaData* interpret() override;
    private:
        LuaString* data;
};
class BooleanLiteral: virtual public Expr {
    public:
        BooleanLiteral(LuaSpecialValue value);
        ~BooleanLiteral();
        LuaData* interpret() override;
    private:
        LuaBoolean* data;
};
class NilLiteral: virtual public Expr {
    public:
        NilLiteral();
        ~NilLiteral();
        LuaData* interpret() override;
    private:
        LuaNil* data;
};


#endif
