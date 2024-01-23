#pragma once

#include "Parser.h"
#include "commons/StmtNode.h"

class StmtParser : public Parser {
   public:
    StmtParser(std::shared_ptr<ParserContext> context);

    std::shared_ptr<StmtNode> parse();
};