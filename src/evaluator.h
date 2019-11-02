#ifndef EVALUATOR_H_INCLUDED
#define EVALUATOR_H_INCLUDED 1
#include "expr.h"
#include "data.h"

class Evaluator {
    public:
        Evaluator(Expr* expr);
        ~Evaluator();
        Data run() const;
    private:
        // This method routes evaluation to the correct evaluation method, depending on the expression type.
        Data evaluate(const Expr* expr) const;

        Data evaluate_binary(const Binary& binary) const;
        Data evaluate_logical(const Logical& logical) const;
        Data evaluate_unary(const Unary& unary) const;
        Data evaluate_grouping(const Grouping& grouping) const;
        Data evaluate_literal(const Literal& literal) const;

        // Checks for truthiness of type, needed in comparison operators, resolving conditions and unary not.
        bool is_truthy(Data data) const;
        bool is_equal(Data left, Data right) const;
        bool is_lesser(Data left, Data right) const;
        
        Expr* expr_; // TODO: This will later be a list of statements. Currently the interpreter is just a calculator.
};



#endif
