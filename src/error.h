#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED 1
#include <exception>
#include <string>

class ParserError : public std::exception {
    public:
        ParserError(const char* filename_, unsigned long line_, const char* info_, const char* where_="");
        virtual const char* what() const throw() override;
        void set_where(const char* where_);
    private:
        std::string filename, line, info, where;
};

class InvalidSyntax : virtual public ParserError {
    public:
        InvalidSyntax(const char* filename_, unsigned long line_);
};

class ExpectedParOpen : virtual public ParserError {
    public:
        ExpectedParOpen(const char* filename_, unsigned long line_, const char* where_="");
};

class ExpectedParClose : virtual public ParserError {
    public:
        ExpectedParClose(const char* filename_, unsigned long line_, const char* where_="");
};

class ExpectedExpr : virtual public ParserError {
    public:
        ExpectedExpr(const char* filename_, unsigned long line_, const char* where_="");
};

class ExpectedKw : virtual public ParserError {
    public:
        ExpectedKw(const char* filename_, unsigned long line_, const char* where_="");
};

class ExpectedVarIdentifier : virtual public ParserError {
    public:
        ExpectedVarIdentifier(const char* filename_, unsigned long line_, const char* where_="");
};

class ExpectedEqualSign : virtual public ParserError {
    public:
        ExpectedEqualSign(const char* filename_, unsigned long line_, const char* where_="");
};


#endif
