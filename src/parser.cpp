#include <string>
#include <vector>
#include <iostream>
#include "token.h"
#include "error.h"
#include "data.h"
#include "parser.h"

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

Expr* Parser::parse() {
    return expression();
}

void Parser::print_tokens() {
    size_t i;
    size_t size = tokens.size();
    for (i = 0; i < size; ++i) {
        cout << "Line " << to_string((tokens.at(i)).line) << ", Token {" << (tokens.at(i)).type << ", " << (tokens.at(i)).value << "}" << endl;
    }
}

ErrorType Parser::get_error() { return error_state; }


// Private functions:
string Parser::read_segment (const char* tokens_stream, unsigned long *i) {
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

Token Parser::peek() {
    return tokens.at(pos);
}

Token Parser::advance() {
    if (!is_at_end()) {
        ++pos;
    }
    return tokens.at(pos-1);
}

bool Parser::is_at_end() {
    return peek().type == "EOS";
}


Expr* Parser::error(string message, ErrorType error) {
    cout << "Parser Error. Line: " << to_string(peek().line) << ". " << message << endl << "Program will not be executed." << endl;
    error_state = error; 
    return NULL; // Mudar isso depois
}


Expr* Parser::expression() {
    return comparison();
}
Expr* Parser::comparison() {
    Expr *expr = addition();
    Expr *right;
    string token_types[6] = {"GREATER", "GREATER_EQUAL", "LESSER", "LESS_EQUAL", "DOUBLE_EQUAL", "NOT_EQUAL"};
    while (check(token_types, 6)) {
        Token op = advance();
        right = addition();
        expr = new Binary(expr, op, right);
    }
    return expr;
}
Expr* Parser::addition() {
    Expr *expr = multiplication();
    Expr *right;
    string token_types[2] = {"MINUS", "PLUS"};
    while (check(token_types, 2)) {
        Token op = advance();
        right = multiplication();
        expr = new Binary(expr, op, right);
    }
    return expr;
}
Expr* Parser::multiplication() {
    Expr *expr = unary();
    Expr *right;
    string token_types[3] = {"SLASH", "STAR", "DOUBLE_SLASH"};
    while (check(token_types, 3)) {
        Token op = advance();
        right = unary();
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
    return primary();
}
Expr* Parser::primary() {
    if (check("KW_FALSE", true)) 
        return new BooleanLiteral(false);
    if (check("KW_TRUE", true)) 
        return new BooleanLiteral(true);
    if (check("KW_NIL", true)) 
        return new NilLiteral();
    if (check("STRING"))
        return new StringLiteral(advance().value);
    if (check("NUMBER"))
        return new NumberLiteral(stod(advance().value));
    if (check("PAR_OPEN")) {
        advance();
        Expr *expr = expression();
        if (!check("PAR_CLOSE", true)) {
            error("Invalid expression: expected closing parenthesis!", EXPECTED_CLOSING_PAR);
        }
        return new Grouping(expr);
    }
    error("Invalid syntax.", INVALID_SYNTAX);
    return new NilLiteral();
}
