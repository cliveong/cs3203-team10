#pragma once

#include "Parser.h"
#include "commons/ExprNode.h"

class RelExprParser : public Parser {
   public:
    RelExprParser(std::shared_ptr<ParserContext> context);

    std::shared_ptr<ExprNode> parse();
};