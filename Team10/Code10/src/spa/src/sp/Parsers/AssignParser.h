#pragma once

#include "Parser.h"
#include "commons/AssignNode.h"
#include "commons/ExprNode.h"
#include "commons/VarNode.h"

class AssignParser : public Parser {
   public:
    AssignParser(std::shared_ptr<ParserContext> context, Token stmtToken);
    std::shared_ptr<AssignNode> parse();

   private:
    Token stmtToken;
};