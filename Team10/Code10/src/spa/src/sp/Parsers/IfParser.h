#pragma once

#include "Parser.h"
#include "commons/ExprNode.h"
#include "commons/IfNode.h"
#include "commons/StmtListNode.h"

class IfParser : public Parser {
   public:
    IfParser(std::shared_ptr<ParserContext> context);

    std::shared_ptr<IfNode> parse();
};