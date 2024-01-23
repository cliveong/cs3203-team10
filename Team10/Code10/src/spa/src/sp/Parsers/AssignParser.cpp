#include "AssignParser.h"

#include "ExprParser.h"

AssignParser::AssignParser(std::shared_ptr<ParserContext> context,
                           Token stmtToken)
    : Parser(context), stmtToken(stmtToken) {}

std::shared_ptr<AssignNode> AssignParser::parse() {
    expect(TokenTypes::ASSIGN, "=");

    auto assignNode = std::make_shared<AssignNode>(context->stmtNum);
    assignNode->addChild(std::make_shared<VarNode>(stmtToken.value, context->stmtNum));

    ExprParser exprParser(context);
    auto exprNode = exprParser.parse();

    assignNode->addChild(exprNode);

    expect(TokenTypes::SEMICOLON, ";");

    return assignNode;
}