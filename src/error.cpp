#include <string>
#include <exception>
#include "error.h"


using namespace std;

ParserError::ParserError (string filename_, unsigned long line_, const char* info_, const char* where_) :
    filename(filename_), line(to_string(line_)), info(info_), where(where_) {}

const char* ParserError::what() const throw() {
    // Static is necessary here. TODO explicar
    static string message = filename+":"+line+": "+"Parser Error: "+info+(where==""? "." : " "+where+".");
    return message.c_str();
}

void ParserError::set_where(const char* where_) { if (where == "") where = where_; }


InvalidSyntax::InvalidSyntax(string filename_, unsigned long line_) :
    ParserError(filename_, line_, "Invalid syntax") {}

ExpectedParOpen::ExpectedParOpen(string filename_, unsigned long line_, const char* where_) :
    ParserError(filename_, line_, "Expected opening parenthesis", where_) {}

ExpectedParClose::ExpectedParClose(string filename_, unsigned long line_, const char* where_) :
    ParserError(filename_, line_, "Expected closing parenthesis", where_) {}

ExpectedExpr::ExpectedExpr(string filename_, unsigned long line_, const char* where_) :
    ParserError(filename_, line_, "Expected expression", where_) {}

ExpectedKw::ExpectedKw(string filename_, unsigned long line_, const char* where_) :
    ParserError(filename_, line_, "Expected keyword", where_) {}

ExpectedVarIdentifier::ExpectedVarIdentifier(string filename_, unsigned long line_, const char* where_) :
    ParserError(filename_, line_, "Expected variable identifier", where_) {}

ExpectedEqualSign::ExpectedEqualSign(string filename_, unsigned long line_, const char* where_) :
    ParserError(filename_, line_, "Expected \"=\"", where_) {}
