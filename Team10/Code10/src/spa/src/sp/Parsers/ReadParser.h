#pragma once

#include "Parser.h"
#include "commons/ReadNode.h"
#include "commons/VarNode.h"

class ReadParser : public Parser {
   public:
    ReadParser(std::shared_ptr<ParserContext> context);

    std::shared_ptr<ReadNode> parse();
};