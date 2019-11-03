#ifndef EVALUATOR_H_INCLUDED
#define EVALUATOR_H_INCLUDED 1
#include "expr.h"
#include "data.h"
#include "visitor.h"

class Evaluator : public ExprVisitor {
    friend class Expr; // Allows Expr to acess visit_xxx methods.
    public:
        Evaluator(Expr* expr);
        ~Evaluator();
        Data run() const;
    private:
        // This method routes evaluation to the correct evaluation method, depending on the expression type.
        Data evaluate(const Expr* expr) const;

        Data visit_binary(const Binary& binary) const override;
        Data visit_logical(const Logical& logical) const override;
        Data visit_unary(const Unary& unary) const override;
        Data visit_grouping(const Grouping& grouping) const override;
        Data visit_literal(const Literal& literal) const override;

        // Checks for truthiness of type, needed in comparison operators, resolving conditions and unary not.
        bool is_truthy(const Data& data) const;
        // The functions below check for equality and "lesser". Together, they are sufficient for all relational operators.
        bool is_equal(const Data& left, const Data& right) const;
        bool is_lesser(const Data& left, const Data& right) const;
        
        Expr* expr_; // TODO: This will later be a list of statements. Currently the interpreter is just a calculator.
};



#endif
