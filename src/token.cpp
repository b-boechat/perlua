#include <string>
#include <iostream>
#include <fstream>
#define SEPARATOR '\0'
using namespace std;

typedef char byte;

struct Token {
    // Tokes are implemented as linked lists
    string type;
    string value;
    unsigned long line;
    Token* next;
};

class Expr {};


class Parser {
    private:
        Token* token;
        string read_segment (byte* tokens_stream, unsigned long *pos) {
            string result = "";
            for (; tokens_stream[*pos] != SEPARATOR; ++(*pos)) {
                result += tokens_stream[*pos];
            }
            ++(*pos);
            return result;
        }
        Expr comparison() {
            // code for comparison expr!
            Expr expr = addition();
            while (match("GREATER", "GREATER_EQUAL", "LESS", "LESS_EQUAL", "DOUBLE_EQUAL", "NOT_EQUAL")) {
            Token algo // ISSO AQUI VAI DAR RUIM !!!!!
            Expr right = addition;
            }

        }
    public:
        Parser(byte* tokens_stream) {
            // This constructor uses "tokens_stream", received from the Perl scanner, to build a linked list of Tokens.
            Token* aux_token;
            string type, value;
            unsigned long line;
            unsigned long pos = 0;
            token = aux_token = NULL;
            while (tokens_stream[pos] != SEPARATOR) {
                type = read_segment(tokens_stream, &pos);
                value = read_segment(tokens_stream, &pos);
                line = stoul(read_segment(tokens_stream, &pos)); // CONVERTER PRA ULONG

                if (!token) {
                // Creating the list
                    token = new Token;
                    token->type = type;
                    token->value = value;
                    token->line = line;
                    token->next = NULL;
                    aux_token = token;
                }
                else {
                // List already exists
                    aux_token->next = new Token;
                    aux_token = aux_token->next;
                    aux_token->type = type;
                    aux_token->value = value;
                    aux_token->line = line;
                    aux_token->next = NULL;
                }
                
            }
        }
        Expr expression() {
            // Em lua, comparison e equality sao juntos: https://www.lua.org/manual/5.1/manual.html 2.5.6
            return comparison();
        }



        void free() {
            Token* aux_token;
            while (token) {
                aux_token = token;
                token = token->next;
                delete(aux_token);
            }
        }
        void print_tokens() {
            Token* aux_token = token;
            while (aux_token) {
                cout << "Line "+to_string(aux_token->line)+", Token {"+aux_token->type+", "+aux_token->value+"}\n";
                aux_token = aux_token->next;
            }
        }
};


int main () {return 0;}
