#pragma once

#include "../commons/ExprNode.h"
#include "../qps/Argument.h"

class ExprArgument : public Argument {
   private:
    std::shared_ptr<ExprNode> exprAST;

   public:
    ExprArgument(ArgType type, std::string value, std::shared_ptr<ExprNode> exprAST);
    bool isFullMatch();
    bool isPartialMatch();
    std::shared_ptr<ExprNode> getExprAST();
};
