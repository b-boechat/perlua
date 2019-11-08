#include <string>
#include <exception>
#include "error.h"

#define MESSAGE_BEGINNING filename+":"+to_string(line)+": "


using namespace std;

const char* ParserError::what() const throw() {return message.c_str();}

InvalidSyntax::InvalidSyntax(string filename, unsigned long line) {
    message = MESSAGE_BEGINNING+"Invalid syntax.";
}

ExpectedParOpen::ExpectedParOpen(string filename, unsigned long line, string where) {
    message = MESSAGE_BEGINNING+"Expected opening parenthesis "+where+".";
}

ExpectedParClose::ExpectedParClose(string filename, unsigned long line, string where) {
    message = MESSAGE_BEGINNING+"Expected closing parenthesis "+where+".";
}

ExpectedExpr::ExpectedExpr(string filename, unsigned long line, string where/*=""*/) {
    // Only appends the ending . if "where" was already specified.
    message = MESSAGE_BEGINNING+"Expected expression "+where+(where==""?"":".");
}
void ExpectedExpr::add_where(string where) {
    message = message+where+".";
}
