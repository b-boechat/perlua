#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED 1
#include <exception>
#include <string>

class ParserError : public std::exception {
    public:
       virtual const char* what() const throw();
       std::string message;
};

class InvalidSyntax : virtual public ParserError {
    public:
        InvalidSyntax(std::string filename, unsigned long line);
};

class ExpectedParOpen : virtual public ParserError {
    public:
        ExpectedParOpen(std::string filename, unsigned long line, std::string where);
};

class ExpectedParClose : virtual public ParserError {
    public:
        ExpectedParClose(std::string filename, unsigned long line, std::string where);
};

class ExpectedExpr : virtual public ParserError {
    public:
        ExpectedExpr(std::string filename, unsigned long line, std::string where="");
        // Sometimes context can only be provided after a few rethrows.
        void add_where(std::string where);
};
#endif
