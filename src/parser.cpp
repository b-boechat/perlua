#include <string>
#include <vector>
#include <iostream>
#include "token.h"
#include "data.h"
#include "error.h"
#include "parser.h"
#include "stmt.h"
#include "expr.h"

#define RETHROW_CLEAN(code) do { \
    try {code} \
    catch (ParserError& err) { \
        delete (expr); \
        if (right) delete (right); \
        throw; \
    } } while (false);

using namespace std;


// Public functions:
Parser::Parser(const char* tokens_stream) {
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

vector <Stmt*> Parser::parse() {
    std::vector <Stmt*> statements;
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

Expr* Parser::expression() {
    return logic_or();
}

Expr* Parser::logic_or() {
    Expr *expr = logic_and();
    Expr *right = NULL;
    while (check("KW_OR")) {
        Token op = advance();
        // The macro below expands to a code dealing with memory management when an exception is propagated through "logic_and()". A macro was defined because this format is used in many expression parsing functions below.
        RETHROW_CLEAN(right = logic_and();)
        /* do {
             try {right = logic_and();}
             catch (ParserError& err) {
               delete (expr);
               if (right) delete (right);
               throw;
           } while (false);
        */
        expr = new Logical(expr, op, right);
    }
    return expr;
}

Expr* Parser::logic_and() {
    Expr *expr = comparison();
    Expr *right = NULL;
    while (check("KW_AND")) {
        Token op = advance();
        RETHROW_CLEAN(right = comparison();)
        expr = new Logical(expr, op, right);
    }
    return expr;
}

Expr* Parser::comparison() {
    Expr *expr = addition();
    Expr *right = NULL;
    string token_types[6] = {"GREATER", "GREATER_EQUAL", "LESSER", "LESS_EQUAL", "DOUBLE_EQUAL", "NOT_EQUAL"};
    while (check(token_types, 6)) {
        Token op = advance();
        RETHROW_CLEAN(right = addition();)
        expr = new Binary(expr, op, right);
    }
    return expr;
}
Expr* Parser::addition() {
    Expr *expr = multiplication();
    Expr *right = NULL;
    string token_types[2] = {"MINUS", "PLUS"};
    while (check(token_types, 2)) {
        Token op = advance();
        RETHROW_CLEAN(right = multiplication();)
        expr = new Binary(expr, op, right);
    }
    return expr;
}
Expr* Parser::multiplication() {
    Expr *expr = unary();
    Expr *right = NULL;
    string token_types[4] = {"SLASH", "STAR", "DOUBLE_SLASH", "PERCENT"};
    while (check(token_types, 4)) {
        Token op = advance();
        RETHROW_CLEAN(right = unary();)
        expr = new Binary(expr, op, right);
    }
    return expr;
}
Expr* Parser::unary() {
    string token_types[3] = {"MINUS", "HASH", "NOT"};
    if (check(token_types, 3)) {
        Token op = advance();
        Expr *right = unary();
        return new Unary(op, right);
    }
    return exponentiation();
}

Expr* Parser::exponentiation() {
    Expr *expr = primary();
    Expr *right;
    if (check("CIRCUMFLEX")) {
        Token op = advance();
        // Here the format is different (because of exponentiation's right associativity) so the macro RETHROW_CLEAN is not used.
        try {right = exponentiation();}
        catch(ParserError& err) {
            delete expr;
            throw;
        }
        expr = new Binary(expr, op, right);
    }
    return expr;
}

Expr* Parser::primary() {
    if (check("KW_FALSE", true)) 
        return new Literal(false);
    if (check("KW_TRUE", true)) 
        return new Literal(true);
    if (check("KW_NIL", true)) 
        return new Literal();
    if (check("STRING"))
        return new Literal(advance().value);
    if (check("NUMBER"))
        return new Literal(stod(advance().value));
    if (check("PAR_OPEN")) {
        advance();
        Expr *expr = expression();
        if (!check("PAR_CLOSE", true))
            throw ExpectedParClose("input.txt", peek().line, "in expression");
        return new Grouping(expr);
    }
    throw ExpectedExpr("input.txt", peek().line);
}

// ===== Statement parsing.

Stmt* Parser::statement() {
    try { return block(); }
    catch (ExpectedEndKw& err) {
        // If block() was called from here, it is an explicit block (not part of if/while/etc statements).
        err.set_where("to close explicit block");
        throw;
    }
}

Stmt* Parser::block() {
    std::vector<Stmt*> statements;
    if (!check("KW_DO", true))
        return empty();
    while (!check("KW_END", true)) {
        if (is_at_end())
            throw ExpectedEndKw("input.txt", peek().line);
        statements.push_back(statement());
    }
    return new Block(statements);
}


Stmt* Parser::empty() {
    if (check("SEMICOLON", true))
        return new Empty();
    return print();
}

Stmt* Parser::print() {
    vector <Expr*> args;
    Token keyword;
    if (!check("FUNC_PRINT")) {
        // TODO Por enquanto eh erro porque só foi implementado até o print, depois trocar isso por um return do proximo stmt e ir propagando a linha de erro até o último statement implementado.
        throw InvalidSyntax("input.txt", peek().line);
    }
    // Saves the "print" keyword token to allow better (runtime) error messages.
    keyword = advance();
    if (!check("PAR_OPEN", true)) {
        throw ExpectedParOpen("input.txt", peek().line, "after \"print\"");
    }
    // This check is included to allow calling print with no arguments.
    if (!check("PAR_CLOSE", true)) {
        do {
            try {args.push_back(expression());}
            catch (ParserError& err) {
                err.set_where("inside \"print\" arguments");
                throw;
            }

        } while (check("COMMA", true));
        if (!check("PAR_CLOSE", true)) {
            // Here, all arguments have been consumed already, so a closing parenthesis is expected.
            throw ExpectedParClose("input.txt", peek().line, "after list of \"print\" arguments");
        }
    }
    return new Print(keyword, args);
}

