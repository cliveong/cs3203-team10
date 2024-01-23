#pragma once

#include "Parser.h"
#include "commons/ExprNode.h"
#include "commons/StmtListNode.h"
#include "commons/WhileNode.h"

class WhileParser : public Parser {
   public:
    WhileParser(std::shared_ptr<ParserContext> context);

    std::shared_ptr<WhileNode> parse();
};