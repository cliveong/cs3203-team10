#include "RelExprParser.h"

#include "RelFactorParser.h"

RelExprParser::RelExprParser(std::shared_ptr<ParserContext> context)
    : Parser(context) {}

std::shared_ptr<ExprNode> RelExprParser::parse() {
    RelFactorParser relFactorParser(context);
    std::shared_ptr<ExprNode> left = relFactorParser.parse();

    Token opToken = getNextToken();

    if (opToken.type == TokenTypes::REL_OPERATORS) {
        std::shared_ptr<ExprNode> opNode = std::make_shared<ExprNode>(
            opToken.value, opToken.value, context->stmtNum);
        opNode->addChild(left);

        std::shared_ptr<ExprNode> right = relFactorParser.parse();

        opNode->addChild(right);
        return opNode;
    } else {
        throw std::runtime_error("Error: Invalid rel expr");
    }
}