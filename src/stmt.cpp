#include <vector>
#include "visitor.h"
#include "stmt.h"

using namespace std;

Stmt::~Stmt() {}

// ===== BLOCK =====

Block::Block(const std::vector <Stmt*> &stmts_) : stmts(stmts_) {}
void Block::accept(StmtVisitor* visitor) const {
    return visitor->visit_block(*(dynamic_cast<const Block*>(this)));
}

// ===== EMPTY =====
Empty::Empty() {}
void Empty::accept(StmtVisitor* visitor) const { 
    return visitor->visit_empty(*(dynamic_cast<const Empty*>(this)));
}
// ===== PRINT =====
Print::Print(Token keyword_, const std::vector <Expr*> &args_) : keyword(keyword_), args(args_) {}
void Print::accept(StmtVisitor* visitor) const { 
    return visitor->visit_print(*(dynamic_cast<const Print*>(this)));
}

// ===== ASSIGNMENT =====
Assignment::Assignment(const std::vector<std::string> &var_list_, const std::vector<Expr*> &expr_list_) : var_list(var_list_), expr_list(expr_list_) {}
void Assignment::accept(StmtVisitor* visitor) const { 
    return visitor->visit_assignment(*(dynamic_cast<const Assignment*>(this)));
}

// ===== DECLARATION =====
Declaration::Declaration(const std::vector<std::string> &var_list_, const std::vector<Expr*> &expr_list_) : var_list(var_list_), expr_list(expr_list_) {}
Declaration::Declaration(const std::vector<std::string> &var_list_) : var_list(var_list_), expr_list() {}
void Declaration::accept(StmtVisitor* visitor) const { 
    return visitor->visit_declaration(*(dynamic_cast<const Declaration*>(this)));
}
