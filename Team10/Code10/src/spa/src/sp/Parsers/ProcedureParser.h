#pragma once

#include "Parser.h"
#include "commons/ProcedureNode.h"
#include "commons/StmtListNode.h"

class ProcedureParser : public Parser {
   public:
    ProcedureParser(std::shared_ptr<ParserContext> context);

    std::shared_ptr<ProcedureNode> parse();
};