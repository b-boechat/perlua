#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED 1
#include <string>

struct Token {
    // Structure to represent source code tokens, which are scanned by the Perl lexer and consumed by the C++ parser.
    std::string type;
    std::string value;
    unsigned long line;
};

#endif
