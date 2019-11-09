#include <string>
#include <exception>
#include "error.h"


using namespace std;

ParserError::ParserError (const char* filename_, unsigned long line_, const char* info_, const char* where_) :
    filename(filename_), line(to_string(line_)), info(info_), where(where_) {}

const char* ParserError::what() const throw() {
    // Static is necessary here. TODO explicar
    static string message = filename+":"+line+": "+"Parser Error: "+info+(where==""? "." : " "+where+".");
    return message.c_str();
}

void ParserError::set_where(const char* where_) { where = where_; }


InvalidSyntax::InvalidSyntax(const char* filename_, unsigned long line_) :
    ParserError(filename_, line_, "Invalid syntax") {}

ExpectedParOpen::ExpectedParOpen(const char* filename_, unsigned long line_, const char* where_) :
    ParserError(filename_, line_, "Expected opening parenthesis", where_) {}

ExpectedParClose::ExpectedParClose(const char* filename_, unsigned long line_, const char* where_) :
    ParserError(filename_, line_, "Expected closing parenthesis", where_) {}

ExpectedExpr::ExpectedExpr(const char* filename_, unsigned long line_, const char* where_) :
    ParserError(filename_, line_, "Expected expression", where_) {}

ExpectedEndKw::ExpectedEndKw(const char* filename_, unsigned long line_, const char* where_) :
    ParserError(filename_, line_, "Expected keyword \"end\"", where_) {}

ExpectedVarIdentifier::ExpectedVarIdentifier(const char* filename_, unsigned long line_, const char* where_) :
    ParserError(filename_, line_, "Expected variable identifier", where_) {}

ExpectedEqualSign::ExpectedEqualSign(const char* filename_, unsigned long line_, const char* where_) :
    ParserError(filename_, line_, "Expected \"=\"", where_) {}
