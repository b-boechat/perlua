#include <string>
#include <exception>
#include "error.h"


using namespace std;

ParserError::ParserError(string filename_, unsigned long line_, const char* info_, const char* where_) :
    filename(filename_), line(to_string(line_)), info(info_), where(where_) {}

const char* ParserError::what() const throw() {
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


RuntimeError::RuntimeError(string filename_) : filename(filename_) {}
string RuntimeError::get_filename() const { return filename; }
string RuntimeError::get_line() const { return line; }
void RuntimeError::set_line(unsigned long line_) {line = to_string(line_);}

InvalidConversion::InvalidConversion(string filename_, const char* type_) : RuntimeError(filename_), type(type_) {}
const char* InvalidConversion::what() const throw() {
    static string message = get_filename()+":"+get_line()+": "+"Runtime Error: "+"Invalid conversion of operand to \""+type+"\".";
    return message.c_str();
}

OutOfRangeConversion::OutOfRangeConversion(string filename_) : RuntimeError(filename_) {}
const char* OutOfRangeConversion::what() const throw() {
    static string message = get_filename()+":"+get_line()+": "+"Runtime Error: "+"Out of range conversion of operand to number.";
    return message.c_str();
}

InvalidTypeForOp::InvalidTypeForOp(string filename_, const char* type_) : RuntimeError(filename_), type(type_) {}
const char* InvalidTypeForOp::what() const throw() {
    static string message = get_filename()+":"+get_line()+": "+"Runtime Error: "+"Attempt to perform incompatible operation on operand of type \""+type+"\".";
    return message.c_str();
}


ComparationDifferentTypes::ComparationDifferentTypes(string filename_) : RuntimeError(filename_) {}
const char* ComparationDifferentTypes::what() const throw() {
    static string message = get_filename()+":"+get_line()+": "+"Runtime Error: "+"Attempt to compare different types.";
    return message.c_str();
}

