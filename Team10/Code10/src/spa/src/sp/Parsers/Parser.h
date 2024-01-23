#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../../commons/ASTNode.h"
#include "../Tokenizer.h"
#include "ParserContext.h"

class Parser {
   protected:
    std::shared_ptr<ParserContext> context;

   public:
    Parser(std::shared_ptr<ParserContext> context);
    Parser(std::vector<Token> tokens);
    std::shared_ptr<ASTNode> parse();

    Token getNextToken();
    Token expect(TokenTypes expectedType, const std::string& expectedValue);
    bool isKeyword(TokenTypes type);

   private:
    std::vector<Token> tokens;
};
