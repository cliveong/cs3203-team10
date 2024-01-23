#include <stdio.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

#include "Parser.h"
#include "ProgramParser.h"
#include "commons/ASTNode.h"

Parser::Parser(std::shared_ptr<ParserContext> context) : context(context) {}
Parser::Parser(std::vector<Token> tokens) : tokens(tokens) {}

std::shared_ptr<ASTNode> Parser::parse() {
    ParserContext parserContext(tokens);
    std::shared_ptr<ParserContext> contextPtr =
        std::make_shared<ParserContext>(parserContext);
    ProgramParser programParser(contextPtr);
    return programParser.parse();
}

Token Parser::getNextToken() {
    if (context->currTokenIndex < context->tokens.size()) {
        return context->tokens[context->currTokenIndex++];
    } else {
        return Token(TokenTypes::EOF_TOKEN, "");
    }
}

Token Parser::expect(TokenTypes expectedType,
                     const std::string& expectedValue) {
    Token nextToken = getNextToken();

    if (expectedType == TokenTypes::LEFT_PAREN) {
        context->parenCounter++;
    }

    if (expectedType == TokenTypes::RIGHT_PAREN) {
        context->parenCounter--;
    }

    if (expectedType == TokenTypes::NAME) {
        if (isKeyword(nextToken.type)) {
            Token newToken(TokenTypes::NAME, nextToken.value);
            context->tokens[int(context->currTokenIndex) - 1] = newToken;
            return newToken;
        }

        if (nextToken.type != expectedType) {
            throw std::runtime_error("Error: Expected variable name got '" +
                                     nextToken.value + "'");
        }

    } else if (nextToken.type != expectedType ||
               nextToken.value != expectedValue) {
        throw std::runtime_error("Error: Expected '" + expectedValue + "'" +
                                 " got '" + nextToken.value + "'");
    }

    return nextToken;
}

bool Parser::isKeyword(TokenTypes type) {
    return (type == TokenTypes::PROCEDURE || type == TokenTypes::PRINT ||
            type == TokenTypes::READ || type == TokenTypes::IF ||
            type == TokenTypes::WHILE || type == TokenTypes::THEN ||
            type == TokenTypes::ELSE || type == TokenTypes::CALL);
}
