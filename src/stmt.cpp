#include <vector>
#include "visitor.h"
#include "stmt.h"

using namespace std;

Stmt::~Stmt() {}

// ===== BLOCK =====

Block::Block(const std::vector <Stmt*> &stmts_) : stmts(stmts_) {}
void Block::accept(const StmtVisitor* visitor) const {
    return visitor->visit_block(*(dynamic_cast<const Block*>(this)));
}

// ===== EMPTY =====
Empty::Empty() {}
void Empty::accept(const StmtVisitor* visitor) const { 
    return visitor->visit_empty(*(dynamic_cast<const Empty*>(this)));
}
// ===== PRINT =====
Print::Print(Token keyword_, const std::vector <Expr*> &args_) : keyword(keyword_), args(args_) {}
void Print::accept(const StmtVisitor* visitor) const { 
    return visitor->visit_print(*(dynamic_cast<const Print*>(this)));
}
