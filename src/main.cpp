#include <string>
#include <vector>
#include <memory>
#include <stdio.h> // strncpy
#include <iostream>
#include <EXTERN.h>
#include <perl.h>

#include "token.h"
#include "data.h"
#include "error.h"
#include "parser.h"
#include "expr.h"
#include "evaluator.h"

#define LEXER_ERROR 1
#define PARSER_ERROR 2
#define RUNTIME_ERROR 3


using namespace std;

PerlInterpreter *my_perl;

char** set_embedding() {
    char** embedding = new char*[4];
    for (int i = 0; i < 3; ++i) {
        // A memoria só aloca até i = 2 porque o embedding[3] é NULL.
        embedding[i] = new char[3];
    }
    strncpy(embedding[0], "", 1);
    strncpy(embedding[1], "-e", 3);
    strncpy(embedding[2], "0", 2);
    embedding[3] = NULL;
    return embedding;
}

void delete_embedding(char** embedding) {
    for (int i = 0; i < 3; ++i)
        delete[](embedding[i]);
    delete[] embedding;
}

bool check_lexer_error(string codified_tokens) {
    string check = "";
    for (size_t i = 0; codified_tokens[i] != '\0'; ++i) {
        check += codified_tokens[i];
    }
    if (check == "ERROR")
        return true;
    return false;
}


int main (int argc, char** argv, char** env) {
    string filename;
    if (argc < 2)
        filename = "input.lua";
    else
        filename = argv[1];
    string lua_code = "use lualexer; $a = return lualexer::tokenize_input(\"" +filename+ "\");";
    char **embedding = set_embedding();
    PERL_SYS_INIT3(&argc, &argv, &env);
    my_perl = perl_alloc();
    perl_construct(my_perl);
    perl_parse(my_perl, NULL, 3, embedding, NULL);
    PL_exit_flags |= PERL_EXIT_DESTRUCT_END;
    perl_run(my_perl);
    SV *val = eval_pv(lua_code.c_str(), TRUE);
    string codified_tokens(SvPV_nolen(val));
    perl_destruct(my_perl);
    perl_free(my_perl);
    PERL_SYS_TERM();
    delete_embedding(embedding);
    if (check_lexer_error(codified_tokens)) {
        exit(LEXER_ERROR);
    }
    Parser parser(filename, codified_tokens.c_str());
//    parser.print_tokens();
    vector<shared_ptr<Stmt> > statements;
    try {
        statements = parser.parse();
    }
    catch (ParserError &err) {
        cout << err.what() << endl;
        exit(PARSER_ERROR);
    }
    Evaluator evaluator(filename, statements);
    try {
        evaluator.run();
    }
    catch (RuntimeError &err) {
        cout << err.what() << endl;
        exit(RUNTIME_ERROR);
    }
    return 0;
}

