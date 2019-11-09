#include <iostream>
#include <vector>
#include <string>
#include <math.h> // floor
#include "token.h"
#include "expr.h"
#include "stmt.h"
#include "data.h"
#include "evaluator.h"

using namespace std;

Evaluator::Evaluator(const std::vector<Stmt*>& statements_) : statements(statements_) {}

Evaluator::~Evaluator() {}

void Evaluator::run() const {
    for (auto it = statements.begin(); it != statements.end(); ++it) {
        execute(*it);
    }
    cout << "\n:)" << endl;
}


// =====
Data Evaluator::evaluate(const Expr* expr) const {
    return expr->accept(this);
}

Data Evaluator::visit_literal(const Literal &literal) const {
    return literal.data;
}

Data Evaluator::visit_grouping(const Grouping &grouping) const {
    return evaluate(grouping.expr);
}

Data Evaluator::visit_unary(const Unary &unary) const {
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

Data Evaluator::visit_logical(const Logical &logical) const {
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

Data Evaluator::visit_binary(const Binary &binary) const {
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

// ===== Expression evaluator helper functions

bool Evaluator::is_truthy(const Data& data) const {
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

bool Evaluator::is_equal(const Data& left, const Data& right) const {
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

bool Evaluator::is_lesser(const Data& left, const Data& right) const {
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

// ===== Statement execution

void Evaluator::execute(const Stmt* stmt) const {
    return stmt->accept(this);
}

void Evaluator::visit_block(const Block& block) const {
    for (auto it = block.stmts.begin(); it != block.stmts.end(); ++it) {
        execute(*it);
    }
}

void Evaluator::visit_empty(const Empty& empty) const {
    // As expected, empty statement does nothing.
    ;
}

void Evaluator::visit_print(const Print& print) const {
    for (auto it = print.args.begin(); it != print.args.end(); ++it) {
        Data data = evaluate(*it);
        string str_arg = stringify(data);
        // By default, Lua "print" separates arguments with a '\t' character and prints a newline after the last one.
        cout << str_arg << '\t';
    }
    cout << endl;
}

string Evaluator::stringify(const Data& data) const {
    LuaType type = data.get_type();
    switch (type) {
        case STRING:
            return STR(data);
        case NIL:
            return string("nil");
        case BOOLEAN:
            return (BOOL(data)? "true" : "false");
        default:
            // TODO entender a precisao certinha do Lua pra numeros.
            return to_string(NUM(data));
    }
    cout << "Should noot be heear d09d0." << endl;
    return "eeeee"; // TODO ajeitar isso ai
}
