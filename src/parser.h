#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED 1

#include <string>
#include <vector>
#include "token.h"
#include "expr.h"
#include "stmt.h"

#define SEPARATOR '\003'

class Parser {
    public:
        /* This constructor uses "tokens_stream", received from the Perl scanner, to build the vector of Tokens.
        It also sets pos to 0, which means we start parsing at the beginning of the vector. */
        Parser(const char* tokens_stream);

        std::vector <Stmt*> parse();
        void print_tokens() const;

    private:
        std::vector <Token> tokens; // Vector of tokens, this is what's going to be parsed.
        unsigned long pos; // Points to current token, while parsing.

        // Helper function used by the constructor.
        std::string read_segment (const char* tokens_stream, unsigned long *i) const;

        // Token vector navigation is done by the helper functions "check", "peek", "advance" and "is_at_end".

        /* Checks if current token type is in "types", returning true if so or false otherwise.
        "types" is passed as an array (C-style) of strings, and "len" is passed as the size of the array.
        If "consume_token" is true, increments "pos" to point to the next token in the vector. */
        bool check(std::string types[], size_t len, bool consume_token=false);
        // Overloads function "check" to allow for single type to be checked.
        bool check(std::string type, bool consume_token=false);

        // Gets current token without advancing "pos".
        Token peek() const;
        // Consumes and returns current token.
        Token advance();

        // Checks if currently at end of input. 
        bool is_at_end() const;

        // Expression parsing is done by the functions "expression", "comparison", "addition" ....

        // expression ->    logic_or
        // logic_or ->      logic_and ("or" logic_and)*
        // logic_and ->     comparison ("and" logic_and)*
        // comparison ->    addition ( ( ">" | ">=" | "<" | "<=" | "==" | "~=" ) addition )* ;
        // addition ->      multiplication ( ( "-" | "+" ) multiplication )*
        // multiplication ->    unary ( ( "/" | "*" | "//" | "%" ) unary )*
        // unary ->             ( ( "-", "#", "not" ) unary ) | exponentiation
        // exponentiation -> (primary "^" exponentiation ) | primary
        // primary ->   "false" | "true" | "nil" | STRING | NUMBER | "(" expression ")" 
        Expr* expression();
        Expr* logic_or();
        Expr* logic_and();
        Expr* comparison();
        Expr* addition();
        Expr* multiplication();
        Expr* unary();
        Expr* exponentiation();
        Expr* primary();

        // Statement parsing.
        
        // program -> (statements)*
        // statement -> block
        // block -> (do (statements)* end) | empty
        // empty -> ";" | print
        // print -> "print" "(" arguments? ")"
        // arguments -> expression ( "," expression)*

        Stmt* block();
        Stmt* statement();        
        Stmt* empty();
        Stmt* print();

};

#endif

