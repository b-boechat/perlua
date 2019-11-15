#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <math.h> // floor
#include <sstream> // ostringstream
#include <iomanip> // setprecision
#include "token.h"
#include "error.h"
#include "environment.h"
#include "expr.h"
#include "stmt.h"
#include "data.h"
#include "evaluator.h"

using namespace std;

#define RETHROW_WITH_LINE(expr, code) do { \
    try {code ;} \
    catch (RuntimeError &err) { \
    err.set_line(expr.op.line); \
    throw; \
    } } while (false);

#define NUM(data) (convert_to_number(data).get_value().lua_num)
#define STR(data) (*(convert_to_string(data).get_value().lua_str))
#define BOOL(data) (data.get_value().lua_bool)

    Evaluator::Evaluator(string filename_, const vector<shared_ptr<Stmt> > &statements_) : filename(filename_), statements(statements_) {
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
        RETHROW_WITH_LINE(unary, return Data(-NUM(right)))
    }
    if (op_type == "HASH") {
        RETHROW_WITH_LINE(unary, return Data(STR(right).size()))
    }
    if (op_type == "KW_NOT") {
        return Data(!is_truthy(right));
    }
    return Data(); // Just to avoid warning, control never gets here.
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
    // To avoid warning
    return Data(); 
}

Data Evaluator::visit_binary(const Binary &binary) const {
    Data left = evaluate(binary.left);
    Data right = evaluate(binary.right);
    string op_type = binary.op.type;
    if (op_type == "DOUBLE_EQUAL")
        return Data(is_equal(left, right));
    if (op_type == "NOT_EQUAL")
        return Data(!is_equal(left, right));

    if (op_type == "LESSER") {
        RETHROW_WITH_LINE(binary, return Data(is_lesser(left, right)))
    }
    if (op_type == "GREATER") {
        RETHROW_WITH_LINE(binary, return Data(is_lesser(right, left)))
    }
    if (op_type == "GREATER_EQUAL") {
        RETHROW_WITH_LINE(binary, return Data(!is_lesser(left, right)))
    }
    if (op_type == "LESSER_EQUAL") {
        RETHROW_WITH_LINE(binary, return Data(!is_lesser(right, left)))
    }
    if (op_type == "DOUBLE_DOT") {
        RETHROW_WITH_LINE(binary, return Data(STR(left) + STR(right)))
    }
    if (op_type == "MINUS") {
        RETHROW_WITH_LINE(binary, return Data(NUM(left) - NUM(right)))
    }
    if (op_type == "PLUS") {
        RETHROW_WITH_LINE(binary, return Data(NUM(left) + NUM(right)))
    }
    if (op_type == "SLASH") {
        // Division by zero is not explicitly treated, following Lua implementation.
        RETHROW_WITH_LINE(binary, return Data(NUM(left) / NUM(right)))
    }
    if (op_type == "STAR") {
        RETHROW_WITH_LINE(binary, return Data(NUM(left) * NUM(right)))

    }
    if (op_type == "DOUBLE_SLASH") {
        RETHROW_WITH_LINE(binary, return Data(floor(NUM(left)/NUM(right))))
    }
    if (op_type == "PERCENT") {
        RETHROW_WITH_LINE(binary, return Data(NUM(left) - floor(NUM(left)/NUM(right)) * NUM(right)))
    }
    if (op_type == "CIRCUMFLEX") {
        RETHROW_WITH_LINE(binary, return Data(pow(NUM(left), NUM(right))))
    }
    // To avoid warning
    return Data();
}

Data Evaluator::visit_variable(const Variable &variable) const {
    return env->get(variable.id.value);
}

// ===== Expression evaluator helper functions

Data Evaluator::convert_to_number(const Data& data) const {
    LuaType type = data.get_type();
    switch (type) {
        case NUMBER:
            return data;
        case STRING:
            try {return Data(stod(STR(data)));}
            catch (invalid_argument) { // stod can throw "invalid_argument" and "out_of_range".
                throw InvalidConversion(filename, "number");
            }
            catch (out_of_range) {
                throw OutOfRangeConversion(filename);
            }
        case BOOLEAN:
            throw InvalidTypeForOp(filename, "boolean");
        default: // case NIL
            throw InvalidTypeForOp(filename, "nil");
    }
}

Data Evaluator::convert_to_string(const Data& data) const {
    LuaType type = data.get_type();
    switch (type) {
        case NUMBER:
            {
                ostringstream out;
                out << setprecision(13) << data.get_value().lua_num;
                return Data(out.str());
            }
        case STRING:
            return data;
        case BOOLEAN:
            throw InvalidTypeForOp(filename, "boolean"); 
        default: // case NIL
            throw InvalidTypeForOp(filename, "nil"); 
    }
}

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
}

bool Evaluator::is_lesser(const Data& left, const Data& right) const {
    LuaType type = left.get_type();
    if (right.get_type() != type)
        throw ComparationDifferentTypes(filename);
    switch (type) {
        case NUMBER:
            return (NUM(left) < NUM(right));
        case STRING:
            return (STR(left).compare(STR(right)) < 0);
        case NIL:
           throw InvalidTypeForOp(filename, "nil");
        default: // CASE BOOLEAN
           throw InvalidTypeForOp(filename, "boolean");
    }
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
    try {
        for (auto it = stmts.begin(); it != stmts.end(); ++it) {
            execute(*it);
        }
    }
    catch (...) {
        delete (env);
        env = previous;
        throw;
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
        string str_arg = stringify_for_print(data);
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
string Evaluator::stringify_for_print(const Data& data) const {
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
            out << setprecision(13) << data.get_value().lua_num;
            return out.str();
    }
}
