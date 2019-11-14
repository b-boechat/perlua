#ifndef STMT_H_INCLUDED
#define STMT_H_INCLUDED 1

#include <vector>
#include <memory>
#include "visitor.h"
#include "token.h"
#include "expr.h"

class Stmt {
    friend class StmtVisitor;
    public:
        virtual ~Stmt();
        virtual void accept(StmtVisitor* visitor) const = 0;
};


class ExplicitBlock : public Stmt {
    public:
        ExplicitBlock(const std::vector<std::shared_ptr<Stmt> > &stmts_);
        void accept(StmtVisitor* visitor) const override;
        const std::vector <std::shared_ptr<Stmt> > stmts;
};

class Empty : public Stmt {
    public:
        Empty();
        void accept(StmtVisitor* visitor) const override;
};

class WhileStmt : public Stmt {
    public:
        WhileStmt(std::shared_ptr<Expr> condition_, const std::vector<std::shared_ptr<Stmt> > &stmts_);
        void accept(StmtVisitor* visitor) const override;
        const std::shared_ptr<Expr> condition;
        const std::vector<std::shared_ptr<Stmt> > stmts;
};

class IfStmt : public Stmt {
    public:
        IfStmt(const std::vector<std::shared_ptr<Expr> > &conditions_, const std::vector<std::vector<std::shared_ptr<Stmt> > > &block_list_); 
        void accept(StmtVisitor* visitor) const override;
        const std::vector<std::shared_ptr<Expr> > conditions;
        const std::vector<std::vector<std::shared_ptr<Stmt> > > block_list;
};

class Print : public Stmt {
    public:
        Print(Token keyword_, const std::vector<std::shared_ptr<Expr> > &args_);
        void accept(StmtVisitor* visitor) const override;
        const Token keyword;
        const std::vector<std::shared_ptr<Expr> > args;
};

class Assignment : public Stmt {
    public:
        Assignment(const std::vector<std::string> &var_list_, const std::vector<std::shared_ptr<Expr> > &expr_list_);
        void accept(StmtVisitor* visitor) const override;
        const std::vector<std::string> var_list;
        const std::vector<std::shared_ptr<Expr> > expr_list;
};

class Declaration : public Stmt {
    public:
        Declaration(const std::vector<std::string> &var_list_, const std::vector<std::shared_ptr<Expr> > &expr_list_);
        Declaration(const std::vector<std::string> &var_list_);
        void accept(StmtVisitor* visitor) const override;
        const std::vector<std::string> var_list;
        const std::vector<std::shared_ptr<Expr> > expr_list;
};
#endif
