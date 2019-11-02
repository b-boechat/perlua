#include <string>
#include "error.h"

//TODO Ajeitar realmente os erros

using namespace std;

TypeError::TypeError(unsigned long line_, string op_, string correct_type_, ErrorCode error_) : line(to_str(line_)), op(op_), correct_type(correct_type_), error(error_) {}

virtual const char* what() const TypeError::throw() {
    string message = "Line "+ line + 
        ". RuntimeError: TypeError! " +
        "Operator '" + op + "' expects " +
        correct_type + "operand!";
    return message.c_str();
}

