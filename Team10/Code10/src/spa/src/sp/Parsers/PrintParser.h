#pragma once

#include "Parser.h"
#include "commons/PrintNode.h"
#include "commons/VarNode.h"

class PrintParser : public Parser {
   public:
    PrintParser(std::shared_ptr<ParserContext> context);

    std::shared_ptr<PrintNode> parse();
};