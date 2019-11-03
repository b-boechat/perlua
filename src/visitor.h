#ifndef VISITOR_H_INCLUDED
#define VISITOR_H_INCLUDED 1

#include "data.h"

class Binary;
class Logical;
class Unary;
class Grouping;
class Literal;

class ExprVisitor {
    friend class Binary;
    friend class Logical;
    friend class Unary;
    friend class Grouping;
    friend class Literal;
    public:
        virtual ~ExprVisitor() {}
    private:
        virtual Data visit_binary(const Binary& binary) const = 0;
        virtual Data visit_logical(const Logical& logical) const = 0;
        virtual Data visit_unary(const Unary& unary) const = 0;
        virtual Data visit_grouping(const Grouping& grouping) const = 0;
        virtual Data visit_literal(const Literal& literal) const = 0;
};


#endif
