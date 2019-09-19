#include <string>
#include <iostream>
#include <vector>
#include <array>
#define SEPARATOR '\0'

using namespace std;

struct Token {
    string type;
    string value;
    unsigned long line;
};

class Expr {};
class Binary : virtual public Expr {
    public:
        Binary(Expr left_, Token op_, Expr right_) {
            left = left_;
            op = op_;
            right = right_;
        }

    private:
        Expr left, right;
        Token op;
};
class Unary : virtual public Expr {
    public:
        Unary(Token op_, Expr right_) {
            op = op_;
            right = right_;
        }

    private:
        Token op;
        Expr right;
};

enum literalType {lua_string, lua_number, lua_boolean, lua_nil_type}; // Implemented types for Lua literals.
enum specialValue {lua_false, lua_true, lua_nil_val}; // Allows treatment of special values.

template <typename T>
class Literal: virtual public Expr {
    // T can be:
    // C++ string        --> Lua string
    // C++ double        --> Lua number
    // C++ specialValue  --> Lua boolean or nil
    public:
        Literal(literalType type_, T value_) {
            type = type_;
            value = value_;
        }
    private:
        literalType type;
        T value;
};




//

class Parser {
    public:
        Parser(char* tokens_stream) {
            /* This constructor uses "tokens_stream", received from the Perl scanner, to build the vector of Tokens.
             It also sets pos to 0, which means we start parsing at the beginning of the vector. */
            pos = 0;
            Token new_token;
            unsigned long i;
            for (i = 0; tokens_stream[i] != SEPARATOR;) {
                new_token.type = read_segment(tokens_stream, &i);
                new_token.value = read_segment(tokens_stream, &i);
                new_token.line = stoul(read_segment(tokens_stream, &i));
                tokens.push_back(new_token); // Append a new token to "tokens"
            }
        }


        void print_tokens() {
            size_t i;
            size_t size = tokens.size();
            for (i = 0; i < size; ++i) {
                cout << "Line " << to_string((tokens.at(i)).line) << ", Token {" << (tokens.at(i)).type << ", " << (tokens.at(i)).value << "}" << endl;
            }
        }
    private:
        vector <Token> tokens; // Vector of tokens, this is what's going to be parsed.
        unsigned long pos; // Points to current token, while parsing.
        string read_segment (char* tokens_stream, unsigned long *i) {
            // Helper function used by the constructor.
            string result = "";
            for (; tokens_stream[*i] != SEPARATOR; ++(*i)) {
                result += tokens_stream[*i];
            }
            ++(*i);
            return result;
        }

        // Token vector navigation is done by the helper functions "check", "peek", "advance" and "is_at_end".

        bool check(string types[], size_t len) {
            // Checks if current token type is in "types", returning true if so or false otherwise.
                // "types" is passed as an array (C-style) of strings, and "len" is passed as the size of the array.
            size_t i;
            for (i = 0; i < len; ++i) {
               if (peek().type == types[i]) {
                    return true;
               }
            }
            return false;
        }
        bool check(string type) {
            // Overloads function "check" to allow for single type to be checked.
            if (peek().type == type)
                return true;
            return false;
        }

        Token peek() {
            // Gets current token without advancing "pos".
            return tokens.at(pos);
        }

        Token advance() {
            // Consumes and returns current token.
            if (!is_at_end()) {
                ++pos;
            }
            return tokens.at(pos-1);
        }

        bool is_at_end() {
            // Checks if currently at end of input. 
            return peek().type == "EOS";
        }

        // Actual expression parsing is done by the functions "expression", "comparison", "addition" ....

        Expr expression() {
            // expression ->    comparison
            return comparison();
        }
        Expr comparison() {
            // comparison ->    addition ( ( ">" | ">=" | "<" | "<=" | "==" | "~=" ) addition )* ;
            Expr expr = addition();
            string token_types[6] = {"GREATER", "GREATER_EQUAL", "LESS", "LESS_EQUAL", "EQUAL", "NOT_EQUAL"};
            while (check(token_types, 6)) {
                Token op = advance();
                Expr right = addition();
                expr = Binary(expr, op, right);
            }
            return expr;
        }
        Expr addition() {
            // addition ->      multiplication ( ( "-" | "+" ) multiplication )*
            Expr expr = multiplication();
            string token_types[2] = {"MINUS", "PLUS"};
            while (check(token_types, 2)) {
                Token op = advance();
                Expr right = multiplication();
                expr = Binary(expr, op, right);
            }
            return expr;
        }
        Expr multiplication() {
            // multiplication ->    unary ( ( "/" | "*" | "//" ) unary )*
            Expr expr = unary();
            string token_types[3] = {"SLASH", "STAR", "DOUBLE_SLASH"};
            while (check(token_types, 3)) {
                Token op = advance();
                Expr right = unary();
                expr = Binary(expr, op, right);
            }
            return expr;
        }
        Expr unary() {
            // unary ->             ( ( "-", "#", "not" ) unary ) | primary
            string token_types[3] = {"MINUS", "HASH", "NOT"};
            if (check(token_types, 3)) {
                Token op = advance();
                Expr right = unary();
                return Unary(op, right);
            }
            return primary();
        }
        Expr primary() {
            // primary ->   NUMBER | STRING | "false" | "true" | "nil" | "(" expression ")" 
            // A IMPLEMENTACAO VAI SER ALGO ASSIM, MAS TEM QUE AVANCAR O POS!!!
            if (check ("FALSE")) return Literal <specialValue> (lua_bool, lua_false);
            if (check ("TRUE")) return Literal <specialValue> (lua_bool, lua_true);

        return Expr(); // Essa linha é placeholder só pra compilar
        }
};


int main (int argc, char** argv) {
    char stream[200] = {'N', 'U', 'M', 'B', 'E', 'R', '\0', '1', '2', '\0', '3', '4', '\0',
                        'S', 'T', 'R', 'I', 'N', 'G', '\0', 'a', 'B', 'c', '\0', '4', '2', '\0',
                        'E', 'O', 'S', '\0', 'N', 'O', 'N', 'E', '\0', '3', '4', '\0'

};


    Parser parser(stream);
    parser.print_tokens();
    return 0;
}
