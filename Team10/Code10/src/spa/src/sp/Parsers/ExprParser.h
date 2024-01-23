#pragma once

#include "Parser.h"
#include "commons/ExprNode.h"

class ExprParser : public Parser {
   public:
    ExprParser(std::shared_ptr<ParserContext> context);

    std::shared_ptr<ExprNode> parse();
};