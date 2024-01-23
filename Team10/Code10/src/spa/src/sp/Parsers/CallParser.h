#pragma once

#include "Parser.h"
#include "ProgramParser.h"
#include "commons/CallNode.h"
#include "commons/ProcedureNode.h"

class CallParser : public Parser {
   public:
    CallParser(std::shared_ptr<ParserContext> context);

    std::shared_ptr<CallNode> parse();
};