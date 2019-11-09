#ifndef VISITOR_H_INCLUDED
#define VISITOR_H_INCLUDED 1

/* This header defines the ExprVisitor and StmtVisitor base classes, from which the Evaluator class inherits. This structure is used to implement the Visitor pattern, allowing for double dispatch.
 */


#include "data.h"

class Binary;
class Logical;
class Unary;
class Grouping;
class Literal;
class Variable;

class ExprVisitor {
    friend class Binary;
    friend class Logical;
    friend class Unary;
    friend class Grouping;
    friend class Literal;
    friend class Variable;
    public:
        virtual ~ExprVisitor() {}
    private:
        virtual Data visit_binary(const Binary& binary) const = 0;
        virtual Data visit_logical(const Logical& logical) const = 0;
        virtual Data visit_unary(const Unary& unary) const = 0;
        virtual Data visit_grouping(const Grouping& grouping) const = 0;
        virtual Data visit_literal(const Literal& literal) const = 0;
        virtual Data visit_variable(const Variable& variable) const = 0;
};

class Block;
class Empty;
class Print;
class Assignment;
class Declaration;

class StmtVisitor {
    friend class Block;
    friend class Empty;
    friend class Print;
    friend class Declaration;
    friend class Assignment;
    public:
        virtual ~StmtVisitor() {}
    private:
        // "visit_block" is not const because it (temporarily) alters the environment.
        virtual void visit_block(const Block& block) = 0;
        virtual void visit_empty(const Empty& empty) const = 0;
        virtual void visit_print(const Print& print) const = 0;
        virtual void visit_declaration(const Declaration& declar) const = 0;
        virtual void visit_assignment(const Assignment& assig) const = 0;
};

#endif
