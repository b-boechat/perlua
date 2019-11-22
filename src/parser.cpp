#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "token.h"
#include "data.h"
#include "error.h"
#include "parser.h"
#include "stmt.h"
#include "expr.h"

using namespace std;


// Public functions:
Parser::Parser(string filename_, const char* tokens_stream) : filename(filename_) {
    pos = 0;
    Token new_token;
    unsigned long i;
    for (i = 0; tokens_stream[i] != '\0' ;) {
        new_token.type = read_segment(tokens_stream, &i);
        new_token.value = read_segment(tokens_stream, &i);
        new_token.line = stoul(read_segment(tokens_stream, &i));
        tokens.push_back(new_token); // Append a new token to "tokens"
    }
}

vector <shared_ptr<Stmt> > Parser::parse() {
    vector <shared_ptr<Stmt> > statements;
    while (!is_at_end()) {
        statements.push_back(statement());
    }
    return statements;
}

void Parser::print_tokens() const {
    size_t i;
    size_t size = tokens.size();
    for (i = 0; i < size; ++i) {
        cout << "Line " << to_string((tokens.at(i)).line) << ", Token {" << (tokens.at(i)).type << ", " << (tokens.at(i)).value << "}" << endl;
    }
}

// Private functions:
string Parser::read_segment (const char* tokens_stream, unsigned long *i) const {
    string result = "";
    for (; tokens_stream[*i] != SEPARATOR; ++(*i)) {
        result += tokens_stream[*i];
    }
    ++(*i);
    return result;
}

bool Parser::check(string types[], size_t len, bool consume_token /*=false*/ ) {
    size_t i;
    for (i = 0; i < len; ++i) {
       if (peek().type == types[i]) {
            if (consume_token)
                ++pos;
            return true;
       }
    }
    return false;
}
bool Parser::check(string type, bool consume_token /*=false*/ ) {
    // Overloads function "check" to allow for single type to be checked.
    if (peek().type == type){
        if (consume_token)
            ++pos;
        return true;
    }
    return false;
}

Token Parser::peek() const {
    return tokens.at(pos);
}

Token Parser::advance() {
    if (!is_at_end()) {
        ++pos;
    }
    return tokens.at(pos-1);
}

bool Parser::is_at_end() const {
    return peek().type == "EOS";
}


// ===== Expression parsing

shared_ptr<Expr> Parser::expression() {
    return logic_or();
}

shared_ptr<Expr> Parser::logic_or() {
    shared_ptr<Expr> expr = logic_and();
    shared_ptr<Expr> right;
    while (check("KW_OR")) {
        Token op = advance();
        right = logic_and();
        expr = make_shared<Logical>(expr, op, right);
    }
    return expr;
}

shared_ptr<Expr> Parser::logic_and() {
    shared_ptr<Expr> expr = comparison();
    shared_ptr<Expr> right;
    while (check("KW_AND")) {
        Token op = advance();
        right = comparison();
        expr = make_shared<Logical>(expr, op, right);
    }
    return expr;
}

shared_ptr<Expr> Parser::comparison() {
    shared_ptr<Expr> expr = concatenation();
    shared_ptr<Expr> right;
    string token_types[6] = {"GREATER", "GREATER_EQUAL", "LESSER", "LESSER_EQUAL", "DOUBLE_EQUAL", "NOT_EQUAL"};
    while (check(token_types, 6)) {
        Token op = advance();
        right = concatenation();
        expr = make_shared<Binary>(expr, op, right);
    }
    return expr;
}

shared_ptr<Expr> Parser::concatenation() {
    shared_ptr<Expr> expr = addition();
    shared_ptr<Expr> right;
    // Concatenation is a right associative binary operation.
    if (check("DOUBLE_DOT")) {
        Token op = advance();
        right = concatenation();
        expr = make_shared<Binary>(expr, op, right);
    }
    return expr;
}

shared_ptr<Expr> Parser::addition() {
    shared_ptr<Expr> expr = multiplication();
    shared_ptr<Expr> right;
    string token_types[2] = {"MINUS", "PLUS"};
    while (check(token_types, 2)) {
        Token op = advance();
        right = multiplication();
        expr = make_shared<Binary>(expr, op, right);
    }
    return expr;
}
shared_ptr<Expr> Parser::multiplication() {
    shared_ptr<Expr> expr = unary();
    shared_ptr<Expr> right;
    string token_types[4] = {"SLASH", "STAR", "DOUBLE_SLASH", "PERCENT"};
    while (check(token_types, 4)) {
        Token op = advance();
        right = unary();
        expr = make_shared<Binary>(expr, op, right);
    }
    return expr;
}
shared_ptr<Expr> Parser::unary() {
    string token_types[3] = {"MINUS", "HASH", "NOT"};
    if (check(token_types, 3)) {
        Token op = advance();
        shared_ptr<Expr> right = unary();
        return make_shared<Unary>(op, right);
    }
    return exponentiation();
}

shared_ptr<Expr> Parser::exponentiation() {
    shared_ptr<Expr> expr = primary();
    shared_ptr<Expr> right;
    // Exponentiation is a right associative binary operation.
    if (check("CIRCUMFLEX")) {
        Token op = advance();
        right = exponentiation();
        expr = make_shared<Binary>(expr, op, right);
    }
    return expr;
}

shared_ptr<Expr> Parser::primary() {
    if (check("KW_FALSE", true)) 
        return make_shared<Literal>(false);
    if (check("KW_TRUE", true)) 
        return make_shared<Literal>(true);
    if (check("KW_NIL", true)) 
        return make_shared<Literal>();
    if (check("STRING"))
        return make_shared<Literal>(advance().value);
    if (check("NUMBER"))
        return make_shared<Literal>(stod(advance().value));
    if (check("IDENTIFIER"))
        return make_shared<Variable>(advance());
    if (check("PAR_OPEN")) {
        advance();
        shared_ptr<Expr> expr = expression();
        if (!check("PAR_CLOSE", true))
            throw ExpectedParClose(filename, peek().line, "in expression");
        return make_shared<Grouping>(expr);
    }
    throw ExpectedExpr(filename, peek().line);
}

// ===== Statement parsing.

shared_ptr<Stmt> Parser::statement() {
    return explicit_block();
}

shared_ptr<Stmt> Parser::explicit_block() {
    if (!check("KW_DO", true))
        return empty();
    vector<shared_ptr<Stmt> > statements;
    try {statements = block("KW_END");}
    catch (ExpectedKw& err) {
        err.set_where("\"end\" to close explicit block");
        throw;
    }
    advance(); // Consumes the KW_END token
    return make_shared<ExplicitBlock>(statements);
}

vector<shared_ptr<Stmt> > Parser::block(const char* delimiter, const char* sec_delimiter/*=""*/, const char* third_delimiter/*=""*/) {
    // Delimiter is the keyword that closes the block. It can be "KW_END", "KW_UNTIL", "KW_ELSEIF" or "KW_ELSE", depending on where this function is called.
    // In Lua grammar, there's a maximum of 3 possible delimiters at once ("KW_END", "KW_ELSEIF" and "KW_ELSE" inside if or elseif blocks)
    // It's important to note that this function does NOT consume the ending token.
    vector<shared_ptr<Stmt> > statements;
    while (! (check(delimiter) or check(sec_delimiter) or check(third_delimiter))) {
        if (is_at_end())
            throw ExpectedKw(filename, peek().line);
        statements.push_back(statement());
    }
    return statements;
}

shared_ptr<Stmt> Parser::empty() {
    if (check("SEMICOLON", true))
        return make_shared<Empty>();
    return while_stmt();
}

shared_ptr<Stmt> Parser::while_stmt() {
    if (!check("KW_WHILE", true))
        return if_stmt();
    shared_ptr<Expr> condition;
    try {condition = expression();}
    catch (ExpectedExpr &err) {
        err.set_where("as \"while\" condition");
        throw;
    }
    if (!check("KW_DO", true))
        throw ExpectedKw(filename, peek().line, "\"do\" after \"while\" condition");
    vector<shared_ptr<Stmt> > statements;
    try {statements = block("KW_END");}
    catch (ExpectedKw &err) {
        err.set_where("\"end\" to close \"while\" block");
        throw;
    }
    advance();
    return make_shared<WhileStmt>(condition, statements);
}

shared_ptr<Stmt> Parser::if_stmt() {
    if (!check("KW_IF", true))
        return print();
    /* If-elif-else structures are stored as vectors of conditions and blocks.
    i = 0 Corresponds to if condition and if block. 
    0 < i < condition_list(size) are elseif condition and blocks.
    block_list has an optional additional else block (in this case, block_list is one unit bigger than condition_list) 
    */
    vector<shared_ptr<Expr> > condition_list;
    vector<vector<shared_ptr<Stmt> > > block_list;
    do {
        // This loop parses an if-elseif structure.
        try {condition_list.push_back(expression());}
        catch (ExpectedExpr &err) {
            err.set_where((block_list.size()? "as \"elseif\" condition" : "as \"if\" condition"));
            throw;
        }
        if (!check("KW_THEN", true)) {
            throw ExpectedKw(filename, peek().line,
                    (block_list.size()? "\"then\" after \"elseif\" condition" : "\"then\" after \"if\" condition"));
        }
        try {block_list.push_back(block("KW_ELSEIF", "KW_ELSE", "KW_END"));}
        catch (ExpectedKw &err) {
            err.set_where((block_list.size()? "\"end\" to close \"elseif\" block" : "\"end\" to close \"if\" block"));
            throw;
        }
    } while (check("KW_ELSEIF", true));
    // Parse an additional else structure.
    if (check("KW_ELSE", true)) {
        try{block_list.push_back(block("KW_END"));}
        catch (ExpectedKw &err) {
            err.set_where("\"end\" to close \"else\" block");
            throw;
        }
    }
    // Consumes "end" token.
    advance();
    return make_shared<IfStmt>(condition_list, block_list);
}

shared_ptr<Stmt> Parser::print() {
    if (!check("FUNC_PRINT"))
        return declaration();
    // Saves the "print" keyword token to allow better (runtime) error messages.
    vector<shared_ptr<Expr> > args;
    Token keyword = advance();
    if (!check("PAR_OPEN", true)) {
        throw ExpectedParOpen(filename, peek().line, "after \"print\"");
    }
    // This check is included to allow calling print with no arguments.
    if (!check("PAR_CLOSE", true)) {
        do {
            try {args.push_back(expression());}
            catch (ExpectedExpr& err) {
                err.set_where("inside \"print\" arguments");
                throw;
            }

        } while (check("COMMA", true));
        if (!check("PAR_CLOSE", true)) {
            // Here, all arguments have been consumed already, so a closing parenthesis is expected.
            throw ExpectedParClose(filename, peek().line, "after list of \"print\" arguments");
        }
    }
    return make_shared<Print>(keyword, args);
}

shared_ptr<Stmt> Parser::declaration() {
    if (!check("KW_LOCAL", true))
        return assignment();
    vector<string> vars;
    vector<shared_ptr<Expr> > exprs;
    // Parses a list of identifiers as lvalues.
    try {vars = var_list();}
    catch(ExpectedVarIdentifier& err) {
        err.set_where("in local declaration variable list");
        throw;
    }
    // In declarations, attributing initial values is optional.
    if (!check("SINGLE_EQUAL", true))
        return make_shared<Declaration>(vars);
    // If an equal sign is found, an expression list is expected.
    try {exprs = exp_list();}
    catch(ExpectedExpr& err) {
        err.set_where("in local declaration expression list");
        throw;
    }
    return make_shared<Declaration>(vars, exprs);
}

shared_ptr<Stmt> Parser::assignment() { 
    if (!check("IDENTIFIER")) {
        throw InvalidSyntax(filename, peek().line);
    }
    vector<string> vars;
    vector<shared_ptr<Expr> > exprs;
    try {vars = var_list();}
    catch(ExpectedVarIdentifier& err) {
        err.set_where("in assignment variable list");
        throw;
    }
    // In assignments, attributing values is mandatory.
    if (!check("SINGLE_EQUAL", true))
        throw ExpectedEqualSign(filename, peek().line, "in assignment"); 
    try {exprs = exp_list();}
    catch(ExpectedExpr& err) {
        err.set_where("in assignment expression list");
        throw;
    }
    return make_shared<Assignment>(vars, exprs);
}

vector<string> Parser::var_list() {
    vector<string> vars;
    do {
        if (!check("IDENTIFIER"))
            throw ExpectedVarIdentifier(filename, peek().line); 
        vars.push_back(advance().value);
    } while (check("COMMA", true));
    return vars;
}

vector<shared_ptr<Expr> > Parser::exp_list() {
    vector<shared_ptr<Expr> > exprs;
    do {
        exprs.push_back(expression());
    } while (check("COMMA", true));
    return exprs;
}


