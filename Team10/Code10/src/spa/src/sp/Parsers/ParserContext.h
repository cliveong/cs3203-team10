#pragma once

#include <unordered_set>
#include <vector>

#include "../Tokenizer.h"

class ParserContext {
   public:
    ParserContext(std::vector<Token> tokens);

    std::vector<Token> tokens;
    int stmtNum;
    int currTokenIndex;
    int parenCounter;
};
