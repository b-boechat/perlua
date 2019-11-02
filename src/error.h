#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED 1
#include <exception>
#include <string>
#include "token.h"
#include "data.h"

// NÃO TA FUNCIONANDO AINDAAA TODO AJEIAR


enum ErrorCode {OK, INVALID_SYNTAX, EXPECTED_CLOSING_PAR};

class RuntimeError : public exception {};

class TypeError : public RuntimeError {
    public:
        TypeError(unsigned long line_, std::string op_, std::string correct_type_, ErrorCode error_);
        virtual const char* what() const throw();
        std::string line, op, correct_type;
        ErrorCode error;
};


#endif
