#pragma once

#include <unordered_set>

#include "Parser.h"
#include "commons/ProcedureNode.h"
#include "commons/ProgramNode.h"

class ProgramParser : public Parser {
   public:
    ProgramParser(std::shared_ptr<ParserContext> context);

    std::shared_ptr<ProgramNode> parse();
};