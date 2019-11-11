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
        virtual void accept(StmtVisitor* visitor) const = 0;
};


class ExplicitBlock : public Stmt {
    public:
        ExplicitBlock(const std::vector<Stmt*> &stmts_);
        ~ExplicitBlock();
        void accept(StmtVisitor* visitor) const override;
        const std::vector <Stmt*> stmts;
};

class Empty : public Stmt {
    public:
        Empty();
        void accept(StmtVisitor* visitor) const override;
};

class WhileStmt : public Stmt {
    public:
        WhileStmt(Expr* condition_, const std::vector<Stmt*> &stmts_);
        ~WhileStmt();
        void accept(StmtVisitor* visitor) const override;
        const Expr* condition;
        const std::vector<Stmt*> stmts;
};

class IfStmt : public Stmt {
    public:
        IfStmt(const std::vector<Expr*> &conditions_, const std::vector<std::vector<Stmt*> > &block_list_); 
        ~IfStmt();
        void accept(StmtVisitor* visitor) const override;
        const std::vector<Expr*> conditions;
        const std::vector<std::vector<Stmt*> > block_list;
};

class Print : public Stmt {
    public:
        Print(Token keyword_, const std::vector<Expr*> &args_);
        ~Print();
        void accept(StmtVisitor* visitor) const override;
        const Token keyword;
        const std::vector<Expr*> args;
};

class Assignment : public Stmt {
    public:
        Assignment(const std::vector<std::string> &var_list_, const std::vector<Expr*> &expr_list_);
        ~Assignment();
        void accept(StmtVisitor* visitor) const override;
        const std::vector<std::string> var_list;
        const std::vector<Expr*> expr_list;
};

class Declaration : public Stmt {
    public:
        Declaration(const std::vector<std::string> &var_list_, const std::vector<Expr*> &expr_list_);
        Declaration(const std::vector<std::string> &var_list_);
        ~Declaration();
        void accept(StmtVisitor* visitor) const override;
        const std::vector<std::string> var_list;
        const std::vector<Expr*> expr_list;
};
#endif
