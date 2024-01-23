#pragma once

#include "Parser.h"
#include "commons/StmtListNode.h"
#include "commons/StmtNode.h"

class StmtListParser : public Parser {
   public:
    StmtListParser(std::shared_ptr<ParserContext> context);

    std::shared_ptr<StmtListNode> parse();
};