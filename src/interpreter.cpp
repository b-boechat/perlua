#include <string>
#include <iostream>
#include <EXTERN.h>
#include <perl.h>


#include "token.h"
#include "data.h"
#include "parser.h"
#include "expr.h"

using namespace std;

PerlInterpreter *my_perl;

int main (int argc, char** argv, char** env) {
    char *embedding[] = {"", "-e", "O", NULL};
    PERL_SYS_INIT3(&argc, &argv, &env);
    my_perl = perl_alloc();
    perl_construct(my_perl);
    perl_parse(my_perl, NULL, 3, embedding, NULL);
    PL_exit_flags |= PERL_EXIT_DESTRUCT_END;
    perl_run(my_perl);
    SV *val = eval_pv("use lualexer; $a = return lualexer::tokenize_input(\"input.txt\");", TRUE);
    string codified_tokens(SvPV_nolen(val));
    //cout << "Len: " << codified_tokens.length() << endl;
    perl_destruct(my_perl);
    perl_free(my_perl);
    PERL_SYS_TERM();
    Parser parser(codified_tokens.c_str());
    //parser.print_tokens();
    Expr *expr = parser.parse();
    LuaData *data = expr->interpret();
    cout << data->eval_number() << endl;
    return 0;

}

