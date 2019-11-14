#include <vector>
#include <memory>
#include "visitor.h"
#include "stmt.h"

using namespace std;

Stmt::~Stmt() {}

// ===== EXPLICIT BLOCK =====
ExplicitBlock::ExplicitBlock(const vector <shared_ptr<Stmt> > &stmts_) : stmts(stmts_) {}
void ExplicitBlock::accept(StmtVisitor* visitor) const {
    return visitor->visit_explicit_block(*(dynamic_cast<const ExplicitBlock*>(this)));
}

// ===== EMPTY =====
Empty::Empty() {}
void Empty::accept(StmtVisitor* visitor) const { 
    return visitor->visit_empty(*(dynamic_cast<const Empty*>(this)));
}

// ===== WHILE STMT =====
WhileStmt::WhileStmt(shared_ptr<Expr> condition_, const vector<shared_ptr<Stmt> > &stmts_) : condition(condition_), stmts(stmts_) {}
void WhileStmt::accept(StmtVisitor* visitor) const { 
    return visitor->visit_while_stmt(*(dynamic_cast<const WhileStmt*>(this)));
}

// ===== IF STMT ===== 
IfStmt::IfStmt(const vector<shared_ptr<Expr> > &conditions_, const vector<vector<shared_ptr<Stmt> > > &block_list_) :
    conditions(conditions_), block_list(block_list_) {}
void IfStmt::accept(StmtVisitor* visitor) const { 
    return visitor->visit_if_stmt(*(dynamic_cast<const IfStmt*>(this)));
}

// ===== PRINT =====
Print::Print(Token keyword_, const vector <shared_ptr<Expr> > &args_) : keyword(keyword_), args(args_) {}
void Print::accept(StmtVisitor* visitor) const { 
    return visitor->visit_print(*(dynamic_cast<const Print*>(this)));
}

// ===== ASSIGNMENT =====
Assignment::Assignment(const vector<string> &var_list_, const vector<shared_ptr<Expr> > &expr_list_) : var_list(var_list_), expr_list(expr_list_) {}
void Assignment::accept(StmtVisitor* visitor) const { 
    return visitor->visit_assignment(*(dynamic_cast<const Assignment*>(this)));
}

// ===== DECLARATION =====
Declaration::Declaration(const vector<string> &var_list_, const vector<shared_ptr<Expr> > &expr_list_) : var_list(var_list_), expr_list(expr_list_) {}
Declaration::Declaration(const vector<string> &var_list_) : var_list(var_list_), expr_list() {}
void Declaration::accept(StmtVisitor* visitor) const { 
    return visitor->visit_declaration(*(dynamic_cast<const Declaration*>(this)));
}
