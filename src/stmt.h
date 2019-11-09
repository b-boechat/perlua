#ifndef STMT_H_INCLUDED
#define STMT_H_INCLUDED 1

#include <vector>
#include "visitor.h"
#include "token.h"
#include "expr.h"

class Stmt {
    friend class StmtVisitor;
    public:
        virtual ~Stmt();
        virtual void accept(const StmtVisitor* visitor) const = 0;
};


class Block : public Stmt {
    public:
        Block(const std::vector<Stmt*> &stmts_);
        void accept(const StmtVisitor* visitor) const override;
        const std::vector <Stmt*> stmts;
};


class Empty : public Stmt {
    public:
        Empty();
        void accept(const StmtVisitor* visitor) const override;
};

class Print : public Stmt {
    public:
        Print(Token keyword_, const std::vector<Expr*> &args_);
        void accept(const StmtVisitor* visitor) const override;
        const Token keyword;
        const std::vector <Expr*> args;
};

class GlobalAssignment : public Stmt {
    public:
        GlobalAssignment(const std::vector<Token> &var_list_, const std::vector<Expr*> &expr_list_);
        void accept(const StmtVisitor* visitor) const override;
        const std::vector<Token> var_list;
        const std::vector<Token> expr_list;
};

#endif
