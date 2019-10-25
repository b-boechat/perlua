#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED 1

#include <string>
#include <vector>
#include "token.h"
#include "expr.h"

#define SEPARATOR '\003'

class Parser {
    public:
        /* This constructor uses "tokens_stream", received from the Perl scanner, to build the vector of Tokens.
        It also sets pos to 0, which means we start parsing at the beginning of the vector. */
        Parser(const char* tokens_stream);

        Expr* parse();
        void print_tokens();

    private:
        std::vector <Token> tokens; // Vector of tokens, this is what's going to be parsed.
        unsigned long pos; // Points to current token, while parsing.
        // Helper function used by the constructor.
        std::string read_segment (const char* tokens_stream, unsigned long *i);

        // Token vector navigation is done by the helper functions "check", "peek", "advance" and "is_at_end".

        /* Checks if current token type is in "types", returning true if so or false otherwise.
        "types" is passed as an array (C-style) of strings, and "len" is passed as the size of the array.
        If "consume_token" is true, increments "pos" to point to the next token in the vector. */
        bool check(std::string types[], size_t len, bool consume_token=false);
        // Overloads function "check" to allow for single type to be checked.
        bool check(std::string type, bool consume_token=false);

        // Gets current token without advancing "pos".
        Token peek();
        // Consumes and returns current token.
        Token advance();

        // Checks if currently at end of input. 
        bool is_at_end();

        // Error handling is done by function "error".

        // Prints an error message to the user and aborts the program.
        void error(std::string message); 

        // Actual expression parsing is done by the functions "expression", "comparison", "addition" ....

        // expression ->    comparison
        // comparison ->    addition ( ( ">" | ">=" | "<" | "<=" | "==" | "~=" ) addition )* ;
        // addition ->      multiplication ( ( "-" | "+" ) multiplication )*
        // multiplication ->    unary ( ( "/" | "*" | "//" ) unary )*
        // unary ->             ( ( "-", "#", "not" ) unary ) | primary
        // primary ->   "false" | "true" | "nil" | STRING | NUMBER | "(" expression ")" 
        Expr* expression();
        Expr* comparison();
        Expr* addition();
        Expr* multiplication();
        Expr* unary();
        Expr* primary();

};

#endif

