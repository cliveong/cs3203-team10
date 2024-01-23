#include "TermParser.h"

#include "FactorParser.h"

TermParser::TermParser(std::shared_ptr<ParserContext> context)
    : Parser(context) {}

std::shared_ptr<ExprNode> TermParser::parse() {
    FactorParser factorParser(context);

    std::shared_ptr<ExprNode> left = factorParser.parse();

    while (true) {
        Token opToken = getNextToken();

        if (opToken.type == TokenTypes::MULTIPLY ||
            opToken.type == TokenTypes::DIVIDE ||
            opToken.type == TokenTypes::MODULO) {
            std::shared_ptr<ExprNode> right = factorParser.parse();

            std::shared_ptr<ExprNode> termNode = std::make_shared<ExprNode>(
                opToken.value, opToken.value, context->stmtNum);
            termNode->addChild(left);
            termNode->addChild(right);
            left = termNode;
        } else {
            context->currTokenIndex--;
            break;
        }
    }

    return left;
}