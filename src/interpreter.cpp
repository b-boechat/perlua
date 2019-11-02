#include <string>
#include <stdio.h> // strncpy
#include <iostream>
#include <EXTERN.h>
#include <perl.h>

#include "token.h"
#include "data.h"
#include "parser.h"
#include "expr.h"
#include "evaluator.h"

using namespace std;

PerlInterpreter *my_perl;

char** set_embedding() {
    char** embedding = new char*[4];
    for (int i = 0; i < 3; ++i) {
        // A memoria só aloca até i = 2 porque o embedding[3] é NULL.
        embedding[i] = new char[2];
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



int main (int argc, char** argv, char** env) {
    char **embedding = set_embedding();
    PERL_SYS_INIT3(&argc, &argv, &env);
    my_perl = perl_alloc();
    perl_construct(my_perl);
    perl_parse(my_perl, NULL, 3, embedding, NULL);
    PL_exit_flags |= PERL_EXIT_DESTRUCT_END;
    perl_run(my_perl);
    SV *val = eval_pv("use lualexer; $a = return lualexer::tokenize_input(\"input.txt\");", TRUE);
    string codified_tokens(SvPV_nolen(val));
    perl_destruct(my_perl);
    perl_free(my_perl);
    PERL_SYS_TERM();
    delete_embedding(embedding);

    Parser parser(codified_tokens.c_str());
    parser.print_tokens();
    Expr *expr = parser.parse();
    cout << "Here." << endl;
    Evaluator evaluator(expr);
    cout << "Here2." << endl;
    Data result = evaluator.run();
    cout << "Now here." << endl;
    cout << "O resultado é ";
    switch (result.get_type()) {
        case NUMBER:
            cout << NUM(result) << endl;
        break;
        case STRING:
            cout << STR(result) << endl;
        case BOOLEAN:
            cout << (BOOL(result) == true? "true" : "false") << endl;
        case NIL:
            cout << "nil" << endl;
    }
    return 0;

}

