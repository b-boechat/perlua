#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <math.h> // floor
#include <sstream> // ostringstream
#include <iomanip> // setprecision
#include "token.h"
#include "environment.h"
#include "expr.h"
#include "stmt.h"
#include "data.h"
#include "evaluator.h"

using namespace std;

Evaluator::Evaluator(const vector<shared_ptr<Stmt> > &statements_) : statements(statements_) {
    // Creates global environment.
    env = new Environment();
}

Evaluator::~Evaluator() {
    Environment* aux = env;
    // Deletes all nested environments.
    while (env) {
        env = env->enclosing;
        delete(aux);
        aux = env;
    }
}

void Evaluator::run() {
    for (auto it = statements.begin(); it != statements.end(); ++it) {
        execute(*it);
    }
    cout << "\n:)" << endl;
}


// =====
Data Evaluator::evaluate(const shared_ptr<Expr> expr) const {
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
    if (op_type == "DOUBLE_DOT") {
        return Data(STR(left) + STR(right));
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

Data Evaluator::visit_variable(const Variable &variable) const {
    return env->get(variable.id.value);
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

void Evaluator::execute(const shared_ptr<Stmt> stmt) {
    return stmt->accept(this);
}

void Evaluator::visit_explicit_block(const ExplicitBlock& block) {
    Environment* previous = env;
    env = new Environment(previous);
    for (auto it = block.stmts.begin(); it != block.stmts.end(); ++it) {
        execute(*it);
    }
    delete(env);
    env = previous;
}

void Evaluator::visit_while_stmt(const WhileStmt& while_stmt) {
    while (is_truthy(evaluate(while_stmt.condition)))
        execute_block(while_stmt.stmts);
}

void Evaluator::visit_if_stmt(const IfStmt& if_stmt) {
    size_t n_condits = if_stmt.conditions.size();
    size_t n_blocks = if_stmt.block_list.size();
    // Iterates through the if-elseif-else blocks.
    for (size_t i = 0; i < n_blocks; ++i) {
        // If i < n_condits, it's an if or elseif block.
        if (i < n_condits) {
            // If the condition is satisfied, executes the block then breaks out of the loop (no more condition is checked)
            if (is_truthy(evaluate(if_stmt.conditions.at(i)))) {
                execute_block(if_stmt.block_list.at(i));
                break;
            }
        }
        // If there is an else block, it gets executed (since all if-elseif conditions have been evaluated as false).
        else
            execute_block(if_stmt.block_list.at(i));
    }
}


void Evaluator::execute_block(const vector<shared_ptr<Stmt> > &stmts) {
    Environment* previous = env;
    // First, a new environment is created, with the previous one enclosing it.
    env = new Environment(previous);
    // Then, all statements are executed within this new environment.
    // TODO this will have to be a try block so the new env is deleted.
    for (auto it = stmts.begin(); it != stmts.end(); ++it) {
        execute(*it);
    }
    // After execution, the block environment is deleted and the previous one is restored.
    // If a "repeat until" statement is added in the future, deletion can't be done here (because the environment only ends after condition evaluation);
    delete(env);
    env = previous;
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

void Evaluator::visit_declaration(const Declaration& declar) const {
    size_t n_declar = declar.var_list.size();
    size_t n_exp = declar.expr_list.size();
    vector<Data> values;
    // All expressions are evaluated before being attributed.
    for (size_t i = 0; i < n_declar; ++i) {
        if (i < n_exp)
            values.push_back(evaluate(declar.expr_list[i]));
        // If there are more variables than expressions, nils are appended.
        // This includes the case where no attribution is made. In this case, n_exp = 0.
        else
            values.push_back(Data());
    }
    for (size_t i = 0; i < n_declar; ++i) {
        env->declare(declar.var_list[i], values[i]);
    }
}

void Evaluator::visit_assignment(const Assignment& assig) const {
    // The number of assignments made is the number of variables on the left side, regardless of how many expressions are in the right side.
    size_t n_assign = assig.var_list.size();
    size_t n_exp = assig.expr_list.size();
    vector<Data> values;
    for (size_t i = 0; i < n_assign; ++i) {
        if (i < n_exp)
            values.push_back(evaluate(assig.expr_list[i]));
        else 
            values.push_back(Data()); 
    }
    for (size_t i = 0; i < n_assign; ++i) {
        env->assign(assig.var_list[i], values[i]);
    }
}


// Statement helper functions.
string Evaluator::stringify(const Data& data) const {
    LuaType type = data.get_type();
    switch (type) {
        case STRING:
            return STR(data);
        case NIL:
            return string("nil");
        case BOOLEAN:
            return (BOOL(data)? "true" : "false");
        default: // case NUMBER
            ostringstream out;
            out << setprecision(13) << NUM(data);
            return out.str();
    }
}
