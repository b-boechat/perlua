#ifndef EVALUATOR_H_INCLUDED
#define EVALUATOR_H_INCLUDED 1
#include <vector>
#include "expr.h"
#include "data.h"
#include "visitor.h"

class Evaluator : public ExprVisitor, public StmtVisitor {
    friend class Expr; // Allows Expr to acess visit_xxx methods.
    public:
        Evaluator(const std::vector<Stmt*>& statements_);
        ~Evaluator();
        void run() const;
    private:
        // This method routes expression evaluation to the correct evaluation method, depending on the expression type.
        Data evaluate(const Expr* expr) const;

        Data visit_binary(const Binary& binary) const override;
        Data visit_logical(const Logical& logical) const override;
        Data visit_unary(const Unary& unary) const override;
        Data visit_grouping(const Grouping& grouping) const override;
        Data visit_literal(const Literal& literal) const override;

        // Expression evaluation helper functions are is_truthy, is_equal and is_lesser.
        bool is_truthy(const Data& data) const;
        bool is_equal(const Data& left, const Data& right) const;
        bool is_lesser(const Data& left, const Data& right) const;

        // This method routes statement execution to the correct method.
        void execute(const Stmt* stmt) const;

        void visit_block(const Block& block) const override;
        void visit_empty(const Empty& empty) const override;
        void visit_print(const Print& print) const override;

        // Statement execution helper functions are stringify
        
        std::string stringify(const Data& data) const;
        
        // TODO doc 
        const std::vector<Stmt*> statements; 
};



#endif
