#pragma once

#include "Parser.h"
#include "commons/ExprNode.h"

class FactorParser : public Parser {
   public:
    FactorParser(std::shared_ptr<ParserContext> context);

    std::shared_ptr<ExprNode> parse();
};