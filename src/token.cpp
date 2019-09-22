#include <string>
#include <iostream>
#include <vector>
#include <math.h> // floor
#define SEPARATOR '\0'

// TODO Adicionar concatenacao de strings com ..

// TODO Remove this later.
using namespace std;

enum LuaDataType {luaString, luaNumber, luaBoolean, luaNilType}; // Represents implemented Lua types.
enum LuaSpecialValue {luaFalse, luaTrue, luaNilValue}; // Represents Lua special values.

struct Token {
    // Structure to represent source code tokens, which are scanned by the Perl lexer and consumed by the C++ parser.
    string type;
    string value;
    unsigned long line;
};

// TODO Declarei aqui so pra conseguir colocar nos return values do LuaData.
class LuaBoolean;

class LuaData {
    // Defines the base class for Lua data types, from which all data types inherit.
    public:
        virtual ~LuaData() {}
        virtual LuaDataType get_type() = 0;
        virtual LuaBoolean* is_truthy() = 0;
        virtual LuaSpecialValue eval_boolean() { cout << "Should not be here! alsf3f3dsd2" << endl; exit(1); return luaFalse; }
        virtual string eval_string() { cout << "Should not be here! fdsdsksfdk2" << endl; exit(1); return ""; }
        virtual double eval_number() { cout << "Should not be here! 32f9ffsdfds2" << endl; exit(1); return 0; }
        virtual LuaSpecialValue eval_nil() { cout << "Should not be here! lc0wqcsd2" << endl; exit(1); return luaNilValue; }
};
class LuaBoolean : public LuaData {
    // Defines the boolean Lua type.
    // TODO: Boolean is defined first here so that "is_truthy" in other types don't result in a compile error. When using headers, move it to after LuaNumber.
    public:
        LuaBoolean(LuaSpecialValue value_) {
            type = luaBoolean;
            value = value_;
        }
        LuaSpecialValue eval_boolean() override { return value; }
        LuaDataType get_type() override { return type; }
        // Truthiness of a boolean type evaluates to a copy of itself, as expected.
        LuaBoolean* is_truthy() override { return new LuaBoolean(value); }
    private:
        LuaDataType type;
        LuaSpecialValue value;
};
class LuaString : public LuaData {
    // Defines the string Lua type.
    public:
        LuaString(string value_) {
            type = luaString;
            value = value_;
        }
        string eval_string() override { return value; }
        LuaDataType get_type() override { return type; }
        // In Lua, all strings are considered "truthy" (including empty string).
        LuaBoolean* is_truthy() override { return new LuaBoolean(luaTrue); }
    private:
        LuaDataType type;
        string value;
};
class LuaNumber : public LuaData {
    // Defines the number Lua type.
    public:
        LuaNumber(double value_) {
            type = luaNumber;
            value = value_;
        }
        double eval_number() override { return value; }
        LuaDataType get_type() override { return type; }
        // In Lua, all numbers are considered "truthy" (including number 0).
        LuaBoolean* is_truthy() override { return new LuaBoolean(luaTrue); }
    private:
        LuaDataType type;
        double value;
};
class LuaNil : public LuaData {
    // Defines the nil Lua type.
    public:
        LuaNil() {
            type = luaNilType;
            value = luaNilValue;
        }
        LuaSpecialValue eval_nil() override { return value; }
        LuaDataType get_type() override { return type; }
        // A nil value is considered "falsy" in Lua.
        LuaBoolean* is_truthy() override { return new LuaBoolean(luaFalse); }
    private:
        LuaDataType type;
        LuaSpecialValue value;
};

class Expr {
    // Defines the base Expr class, from which all expression types inherit.
    public:
        // This function is overrided and is never called as is.
        virtual LuaData* interpret() = 0;
        virtual ~Expr() {}; // https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
};

class Binary : virtual public Expr {
    public:
        Binary(Expr* left_, Token op_, Expr* right_) : left(left_), right(right_), op(op_) {}
        ~Binary() { delete (left); delete (right); }
        LuaData* interpret() override {
            LuaData *leftData = left->interpret();
            LuaData *rightData = right->interpret();
            // Binary '-' expects two number operands and returns their subtraction. 
            if (op.type == "MINUS") return new LuaNumber(leftData->eval_number() - rightData->eval_number());
            // Binary '+' expects two number operands and returns their sum.
            else if (op.type == "PLUS") return new LuaNumber(leftData->eval_number() + rightData->eval_number());
            // Binary '*' expects two number operands and returns their multiplication
            else if (op.type == "STAR") return new LuaNumber(leftData->eval_number() * rightData->eval_number());
            // Binary '/' expects two number operands and returns their division.
            else if (op.type == "SLASH") return new LuaNumber(leftData->eval_number() / rightData->eval_number());
            // Binary '//' expects two number operands and returns their integer division.
            else if (op.type == "DOUBLE_SLASH") return new LuaNumber(floor(leftData->eval_number() / rightData->eval_number()) );
            // Binary '==' returns equality between two values. Equality in Lua is explained in helper function "isEqual".
            else if (op.type == "DOUBLE_EQUAL") return new LuaBoolean(isEqual(leftData, rightData));
            // Binary '~=' returns opposite of equality between two values.
            else if (op.type == "NOT_EQUAL") return new LuaBoolean(isNotEqual(leftData, rightData));
            // Falta implementar os comparadores.
            cout << "Se nao for comparador, nao era pra tar aqui!" << endl << "op.type = " << op.type << endl;
            return new LuaBoolean(luaTrue);
        }
    private:
        LuaSpecialValue isEqual(LuaData *left, LuaData *right) {
            // Always returns false if operands are of different types. Otherwise, compares operands by value.
            LuaDataType leftType = left->get_type();
            if (leftType != right->get_type())
                return luaFalse;
            switch (leftType) {
                case luaNumber:
                    return (left->eval_number() == right->eval_number()? luaTrue : luaFalse);
                break;
                case luaString:
                    return (left->eval_string() == right->eval_string()? luaTrue : luaFalse);
                break;
                case luaBoolean:
                    return (left->eval_boolean() == right->eval_boolean()? luaTrue : luaFalse);
                break;
                case luaNilType:
                    return luaTrue;    
            }
            cout << "Should not be here! sad2109dsç" << endl;
            return luaTrue;
        }
        LuaSpecialValue isNotEqual(LuaData *left, LuaData *right) {
            // Returns the Lua boolean opposite of "isEqual".
            return (isEqual(left, right) == luaTrue? luaFalse : luaTrue);
        }
        Expr *left, *right;
        Token op;
};
class Unary : virtual public Expr {
    public:
        Unary(Token op_, Expr* right_) : op(op_), right(right_) {}
        ~Unary() {delete (right);}
        LuaData* interpret() override {
            LuaData *rightData = right->interpret();
            // Unary '-' expects a number as operand and returns its additive inverse.
            if (op.type == "MINUS") return new LuaNumber(-(rightData->eval_number()) );
            // Unary '#' expects a string as operand and returns its length.
            else if (op.type == "HASH") return new LuaNumber(rightData->eval_string().size());
            // Unary 'not' returns true if operand is "falsy" (opposite of "truthy"), and returns false otherwise. "Falsy" values in Lua are "false" and "nil". 
            else if (op.type == "NOT") return rightData->is_truthy();

            // TODO remover isso aqui
            cout << "Should not be here! GDSFA";
            exit (1);
        }
    private:
        Token op;
        Expr *right;
};

class Grouping : virtual public Expr {
    public:
        Grouping(Expr* expr_) : expr(expr_) {}
        ~Grouping() {delete (expr);}
        LuaData* interpret() override {
            return expr->interpret();
        }
    private:
        Expr *expr;
};

// "NumberLiteral", "StringLiteral", "BooleanLiteral" and "NilLiteral" defines Lua literal expressions, which consists of single explicit values.
// Eg: 120, "abc", true, nil
class NumberLiteral: virtual public Expr {
    public:
        NumberLiteral(double value) {data = new LuaNumber(value);}
        ~NumberLiteral() {delete(data);}
        LuaData* interpret() override {
            return data;
        }
    private:
        LuaNumber* data;
};
class StringLiteral: virtual public Expr {
    public:
        StringLiteral(string value) {data = new LuaString(value);}
        ~StringLiteral() {delete(data);}
        LuaData* interpret() override {
            return data;
        }
    private:
        LuaString* data;
};
class BooleanLiteral: virtual public Expr {
    public:
        BooleanLiteral(LuaSpecialValue value) {data = new LuaBoolean(value);}
        ~BooleanLiteral() {delete(data);}
        LuaData* interpret() override {
            return data;
        }
    private:
        LuaBoolean* data;
};
class NilLiteral: virtual public Expr {
    public:
        NilLiteral() {data = new LuaNil();}
        ~NilLiteral() {delete(data);}
        LuaData* interpret() override {
            return data;
        }
    private:
        LuaNil* data;
};


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

        Expr* parse() {
            return expression();
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

        bool check(string types[], size_t len, bool consume_token=false) {
            // Checks if current token type is in "types", returning true if so or false otherwise.
            // "types" is passed as an array (C-style) of strings, and "len" is passed as the size of the array.
            // If "consume_token" is true, increments "pos" to point to the next token in the vector.
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
        bool check(string type, bool consume_token=false) {
            // Overloads function "check" to allow for single type to be checked.
            if (peek().type == type){
                if (consume_token)
                    ++pos;
                return true;
            }
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

        // Error handling is done by function "error".

        void error(string message) {
            // Prints an error message to the user and aborts the program.
            cout << "Parser Error. Line: " << to_string(peek().line) << ". " << message << endl << "Parsing aborted, program will not be executed." << endl;
            exit (1); // Mudar isso depois
        }

        // Actual expression parsing is done by the functions "expression", "comparison", "addition" ....

        Expr* expression() {
            // expression ->    comparison
            return comparison();
        }
        Expr* comparison() {
            // comparison ->    addition ( ( ">" | ">=" | "<" | "<=" | "==" | "~=" ) addition )* ;
            Expr *expr = addition();
            Expr *right;
            string token_types[6] = {"GREATER", "GREATER_EQUAL", "LESS", "LESS_EQUAL", "EQUAL", "NOT_EQUAL"};
            while (check(token_types, 6)) {
                Token op = advance();
                right = addition();
                expr = new Binary(expr, op, right);
            }
            return expr;
        }
        Expr* addition() {
            // addition ->      multiplication ( ( "-" | "+" ) multiplication )*
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
        Expr* multiplication() {
            // multiplication ->    unary ( ( "/" | "*" | "//" ) unary )*
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
        Expr* unary() {
            // unary ->             ( ( "-", "#", "not" ) unary ) | primary
            string token_types[3] = {"MINUS", "HASH", "NOT"};
            if (check(token_types, 3)) {
                Token op = advance();
                Expr *right = unary();
                return new Unary(op, right);
            }
            return primary();
        }
        Expr* primary() {
            // primary ->   "false" | "true" | "nil" | STRING | NUMBER | "(" expression ")" 
            if (check("FALSE", true)) 
                return new BooleanLiteral(luaFalse);
            if (check("TRUE", true)) 
                return new BooleanLiteral(luaTrue);
            if (check("NIL", true)) 
                return new NilLiteral();
            if (check("STRING"))
                return new StringLiteral(advance().value);
            if (check("NUMBER"))
                return new NumberLiteral(stod(advance().value));
            if (check("LEFT_PAREN")) {
                // Consumes a left parenthesis and tries to parse a grouping expression.
                advance();
                Expr *expr = expression();
                // Attempts to consume a RIGHT_PAREN from input, otherwise an error is found.
                if (!check("RIGHT_PAREN", true)) {
                    error("Invalid expression: expected closing parenthesis!");
                }
                return new Grouping(expr);
            }
            error("Expected expression!");
            return new NilLiteral();
        }
};


int main (int argc, char** argv) {
    char stream[200] = {'N', 'U', 'M', 'B', 'E', 'R', '\0', '1', '2', '\0', '3', '4', '\0',                                      // 12
                        'M', 'I', 'N', 'U', 'S', '\0', 'N', 'O', 'N', 'E', '\0', '3', '5', '\0',                                 // -
                        'N', 'U', 'M', 'B', 'E', 'R', '\0', '2', '2', '8', '\0', '2', '3', '\0',                                 // 228
                        'S', 'L', 'A', 'S', 'H', '\0', 'N', 'O', 'N', 'E', '\0', '3', '5', '\0',                                 // /
                        'L', 'E', 'F', 'T', '_', 'P', 'A', 'R', 'E', 'N', '\0', 'N', 'O', 'N', 'E', '\0', '3', '5', '\0',        // (
                        'N', 'U', 'M', 'B', 'E', 'R', '\0', '2', '\0', '4', '2', '\0',                                           // 2
                        'S', 'T', 'A', 'R', '\0', 'N', 'O', 'N', 'E', '\0', '4', '2', '\0',                                      // *
                        'N', 'U', 'M', 'B', 'E', 'R', '\0', '4', '\0', '4', '2', '\0',                                           // 4
                        'R', 'I', 'G', 'H', 'T', '_', 'P', 'A', 'R', 'E', 'N', '\0', 'N', 'O', 'N', 'E', '\0', '3', '5', '\0',   // )

                        'E', 'O', 'S', '\0', 'N', 'O', 'N', 'E', '\0', '3', '4', '\0'
    };


    Parser parser(stream);
    parser.print_tokens();
    Expr *expr = parser.parse();
    LuaData *data = expr->interpret();
    cout << data->get_type() << endl;
    cout << data->eval_number() << endl;
    return 0;
}
