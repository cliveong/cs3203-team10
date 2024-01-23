#pragma once

#include "Parser.h"
#include "commons/ExprNode.h"

class RelFactorParser : public Parser {
   public:
    RelFactorParser(std::shared_ptr<ParserContext> context);

    std::shared_ptr<ExprNode> parse();
};