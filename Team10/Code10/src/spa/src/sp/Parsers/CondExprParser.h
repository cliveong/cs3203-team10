#pragma once

#include "Parser.h"
#include "commons/ExprNode.h"

class CondExprParser : public Parser {
   public:
    CondExprParser(std::shared_ptr<ParserContext> context);

    std::shared_ptr<ExprNode> parse();

   private:
    std::shared_ptr<ExprNode> validateNotExpr();
    std::shared_ptr<ExprNode> validateParenExpr();
};