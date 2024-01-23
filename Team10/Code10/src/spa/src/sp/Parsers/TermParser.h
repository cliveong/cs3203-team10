#pragma once

#include "Parser.h"
#include "commons/ExprNode.h"

class TermParser : public Parser {
   public:
    TermParser(std::shared_ptr<ParserContext> context);

    std::shared_ptr<ExprNode> parse();
};