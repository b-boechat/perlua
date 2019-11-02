#include <iostream>
#include <string>
#include <math.h> // floor
#include "token.h"
#include "expr.h"
#include "data.h"
#include "evaluator.h"

using namespace std;

Evaluator::Evaluator(Expr* expr): expr_(expr) {}

Evaluator::~Evaluator() { if (expr_) delete (expr_); }

Data Evaluator::run() const {
    return evaluate(expr_);
}

Data Evaluator::evaluate(const Expr* expr) const {
    ExprType type = expr->get_type();
    switch (type) {
        case BINARY:
            return evaluate_binary(*(dynamic_cast<const Binary*>(expr)));
        case LOGICAL:
            return evaluate_logical(*(dynamic_cast<const Logical*>(expr)));
        case UNARY:
            return evaluate_unary(*(dynamic_cast<const Unary*>(expr)));
        case GROUPING:
            return evaluate_grouping(*(dynamic_cast<const Grouping*>(expr)));
        default: // case LITERAL
            return evaluate_literal(*(dynamic_cast<const Literal*>(expr)));
    }
}

Data Evaluator::evaluate_literal(const Literal &literal) const {
    return literal.data;
}

Data Evaluator::evaluate_grouping(const Grouping &grouping) const {
    return evaluate(grouping.expr);
}

Data Evaluator::evaluate_unary(const Unary &unary) const {
    Data right = evaluate(unary.right); 
    string op_type = unary.op.type;
    if (op_type == "MINUS") {
        // TODO Fazer as checagens de tipo corretamente
        if (right.get_type() != NUMBER) cout << "Tipo errado!!! 093kfspd" << endl;
        //
        return Data(-NUM(right));
    }
    if (op_type == "HASH") {
        // TODO Fazer as checagens de tipo corretamente
        if (right.get_type() != STRING) cout << "Tipo errado!!! 09dqdofd" << endl;
        //
        return Data(STR(right).size());
    }
    if (op_type == "KW_NOT") {
        return Data(!is_truthy(right));
    }
    cout << "Should not be here!! ad2109ddk" << endl;
    return Data();
}

Data Evaluator::evaluate_logical(const Logical &logical) const {
    Data left = evaluate(logical.left);
    string op_type = logical.op.type;
    if (op_type == "KW_OR") {
        return (is_truthy(left)? Data(left) : evaluate(logical.right) );
    }
    if (op_type == "KW_AND") {
        return (is_truthy(left)? evaluate(logical.right) : Data(left));
    }
    cout << "should never be here! 0d30d" << endl;
    return Data();
}

Data Evaluator::evaluate_binary(const Binary &binary) const {
    Data left = evaluate(binary.left);
    Data right = evaluate(binary.right);
    string op_type = binary.op.type;
    if (op_type == "DOUBLE_EQUAL") {
        return Data(is_equal(left, right));
    }
    if (op_type == "NOT_EQUAL") {
        return Data(!is_equal(left, right));
    }
    if (op_type == "LESSER") {
        return Data(is_lesser(left, right));
    }
    if (op_type == "GREATER") {
        // TODO tipo em todos os relacionais
        return Data(is_lesser(right, left));
    }
    if (op_type == "GREATER_EQUAL") {
        return Data(!is_lesser(left, right));
    }
    if (op_type == "LESSER_EQUAL") {
        return Data(!is_lesser(right, left));
    }
    if (op_type == "MINUS") {
        // TODO tipo
        return Data(NUM(left) - NUM(right));
    }
    if (op_type == "PLUS") {
        // TODO tipo
        return Data(NUM(left) + NUM(right));
    }
    if (op_type == "SLASH") {
        // TODO tipo. TODO tratar divisão por 0.
        return Data(NUM(left) / NUM(right));

    }
    if (op_type == "STAR") {
        // TODO tipo
        return Data(NUM(left) * NUM(right));

    }
    if (op_type == "DOUBLE_SLASH") {
        // TODO tipo. TODO tratar divisão por zero.
        return Data(floor(NUM(left)/NUM(right)));
    }
    if (op_type == "PERCENT") {
        // TODO tipo. TODO tratar divisão por zero.
        return Data(NUM(left) - floor(NUM(left)/NUM(right)) * NUM(right));
    }
    if (op_type == "CIRCUMFLEX") {
        // TODO tipo. TODO tratar 0^0.
        return Data(pow(NUM(left), NUM(right)));
    }
    cout << "Should not be here!! ldldd0220kdddk" << endl;
    return Data();
}




bool Evaluator::is_truthy(Data data) const {
    // In Lua, everything is truthy except for false and nil. This includes number 0 and empty strings.
    LuaType type = data.get_type();
    switch (type) {
        case BOOLEAN:
            return data.get_value().lua_bool;
        case STRING:
        case NUMBER:
            return true;
        default: // case NIL
            return false;
    }     
    cout << "Should not be here! alad02dk" << endl;
    return false;
}

bool Evaluator::is_equal(Data left, Data right) const {
    LuaType type = left.get_type();
    if (right.get_type() != type)
        return false;
    switch (type) {
        case STRING:
            return (STR(left) == STR(right));
        case NUMBER:
            return (NUM(left) == NUM(right));
        case BOOLEAN:
            return (BOOL(left) == BOOL(right));
        default: // case NIL
            return true;
    }
    cout << "Should nooout be hear! lsad0dd" << endl;
    return false;
}

bool Evaluator::is_lesser(Data left, Data right) const {
    // TODO Checagem de tipo aqui ou lá
    LuaType type = left.get_type();
    switch (type) {
        case NUMBER:
            return (NUM(left) < NUM(right));
        case STRING:
            return (STR(left).compare(STR(right)) < 0);
        case NIL:
           // TODO erro aqui 
        default: // CASE BOOLEAN
            // TODO erro aqui
            return false;
    }
    cout << "Should noot be heri dsal02" << endl;
    return false;
}

