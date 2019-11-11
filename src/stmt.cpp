#include <vector>
#include "visitor.h"
#include "stmt.h"

Stmt::~Stmt() {}

// ===== EXPLICIT BLOCK =====
ExplicitBlock::ExplicitBlock(const std::vector <Stmt*> &stmts_) : stmts(stmts_) {}
void ExplicitBlock::accept(StmtVisitor* visitor) const {
    return visitor->visit_explicit_block(*(dynamic_cast<const ExplicitBlock*>(this)));
}
ExplicitBlock::~ExplicitBlock() {
    for (auto it = stmts.begin(); it != stmts.end(); ++it) {
        delete (*it);
    }
}

// ===== EMPTY =====
Empty::Empty() {}
void Empty::accept(StmtVisitor* visitor) const { 
    return visitor->visit_empty(*(dynamic_cast<const Empty*>(this)));
}

// ===== WHILE STMT =====
WhileStmt::WhileStmt(Expr* condition_, const std::vector<Stmt*> &stmts_) : condition(condition_), stmts(stmts_) {}
WhileStmt::~WhileStmt() {
    delete(condition);
    for (auto it = stmts.begin(); it != stmts.end(); ++it) {
        delete (*it);
    }
}
void WhileStmt::accept(StmtVisitor* visitor) const { 
    return visitor->visit_while_stmt(*(dynamic_cast<const WhileStmt*>(this)));
}

// ===== IF STMT ===== 
IfStmt::IfStmt(const std::vector<Expr*> &conditions_, const std::vector<std::vector<Stmt*> > &block_list_) :
    conditions(conditions_), block_list(block_list_) {}
IfStmt::~IfStmt() {
    for (auto it = conditions.begin(); it != conditions.end(); ++it) {
        delete (*it);
    }
    for (auto oit = block_list.begin(); oit != block_list.end(); ++oit) {
        for (auto it = (*oit).begin(); it != (*oit).end(); ++it)
            delete (*it);
    }
}
void IfStmt::accept(StmtVisitor* visitor) const { 
    return visitor->visit_if_stmt(*(dynamic_cast<const IfStmt*>(this)));
}

// ===== PRINT =====
Print::Print(Token keyword_, const std::vector <Expr*> &args_) : keyword(keyword_), args(args_) {}
Print::~Print() {
    for (auto it = args.begin(); it != args.end(); ++it) {
        delete (*it);
    }
}
void Print::accept(StmtVisitor* visitor) const { 
    return visitor->visit_print(*(dynamic_cast<const Print*>(this)));
}

// ===== ASSIGNMENT =====
Assignment::Assignment(const std::vector<std::string> &var_list_, const std::vector<Expr*> &expr_list_) : var_list(var_list_), expr_list(expr_list_) {}
Assignment::~Assignment() {
    for (auto it = expr_list.begin(); it != expr_list.end(); ++it) {
        delete (*it);
    }
}
void Assignment::accept(StmtVisitor* visitor) const { 
    return visitor->visit_assignment(*(dynamic_cast<const Assignment*>(this)));
}

// ===== DECLARATION =====
Declaration::Declaration(const std::vector<std::string> &var_list_, const std::vector<Expr*> &expr_list_) : var_list(var_list_), expr_list(expr_list_) {}
Declaration::Declaration(const std::vector<std::string> &var_list_) : var_list(var_list_), expr_list() {}
Declaration::~Declaration() {
    for (auto it = expr_list.begin(); it != expr_list.end(); ++it) {
        delete (*it);
    }
}
void Declaration::accept(StmtVisitor* visitor) const { 
    return visitor->visit_declaration(*(dynamic_cast<const Declaration*>(this)));
}
