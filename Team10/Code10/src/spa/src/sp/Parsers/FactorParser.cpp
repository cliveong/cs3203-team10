#include "FactorParser.h"

#include "../../commons/Constants.h"
#include "ExprParser.h"

FactorParser::FactorParser(std::shared_ptr<ParserContext> context)
    : Parser(context) {}

std::shared_ptr<ExprNode> FactorParser::parse() {
    Token factorToken = getNextToken();
    std::shared_ptr<ExprNode> factorNode;

    if (factorToken.type == TokenTypes::NAME || isKeyword(factorToken.type)) {
        factorNode = std::make_shared<ExprNode>(Constants::VAR, factorToken.value,
                                                context->stmtNum);
    } else if (factorToken.type == TokenTypes::INTEGER) {
        factorNode = std::make_shared<ExprNode>(Constants::CONST, factorToken.value,
                                                context->stmtNum);
    } else if (factorToken.type == TokenTypes::LEFT_PAREN) {
        context->parenCounter++;
        ExprParser exprParser(context);
        factorNode = exprParser.parse();
        expect(TokenTypes::RIGHT_PAREN, ")");
    } else {
        throw std::runtime_error("Error: Invalid expr");
    }

    Token nextToken = getNextToken();
    if (nextToken.type == TokenTypes::LEFT_PAREN) {
        throw std::runtime_error("Error: Unexpected '('");
    } else if (nextToken.type == TokenTypes::NAME ||
               nextToken.type == TokenTypes::INTEGER) {
        throw std::runtime_error("Error: Unexpected token " + nextToken.value);
    } else {
        context->currTokenIndex--;
    }
    return factorNode;
}
