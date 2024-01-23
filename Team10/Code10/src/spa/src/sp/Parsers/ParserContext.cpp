#include "ParserContext.h"

#include <unordered_set>

ParserContext::ParserContext(std::vector<Token> tokens)
    : tokens(tokens), stmtNum(1), currTokenIndex(0), parenCounter(0) {
}