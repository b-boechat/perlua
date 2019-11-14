#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED 1
#include <exception>
#include <string>

class ParserError : public std::exception {
    public:
        ParserError(std::string filename_, unsigned long line_, const char* info_, const char* where_="");
        virtual const char* what() const throw() override;
        void set_where(const char* where_);
    private:
        std::string filename, line, info, where;
};

class InvalidSyntax : virtual public ParserError {
    public:
        InvalidSyntax(std::string filename_, unsigned long line_);
};

class ExpectedParOpen : virtual public ParserError {
    public:
        ExpectedParOpen(std::string filename_, unsigned long line_, const char* where_="");
};

class ExpectedParClose : virtual public ParserError {
    public:
        ExpectedParClose(std::string filename_, unsigned long line_, const char* where_="");
};

class ExpectedExpr : virtual public ParserError {
    public:
        ExpectedExpr(std::string filename_, unsigned long line_, const char* where_="");
};

class ExpectedKw : virtual public ParserError {
    public:
        ExpectedKw(std::string filename_, unsigned long line_, const char* where_="");
};

class ExpectedVarIdentifier : virtual public ParserError {
    public:
        ExpectedVarIdentifier(std::string filename_, unsigned long line_, const char* where_="");
};

class ExpectedEqualSign : virtual public ParserError {
    public:
        ExpectedEqualSign(std::string filename_, unsigned long line_, const char* where_="");
};


#endif
