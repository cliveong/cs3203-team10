#include "WhileParser.h"

#include "../../commons/Constants.h"
#include "CondExprParser.h"
#include "StmtListParser.h"

WhileParser::WhileParser(std::shared_ptr<ParserContext> context)
    : Parser(context) {}

std::shared_ptr<WhileNode> WhileParser::parse() {
    std::shared_ptr<WhileNode> whileNode =
        std::make_shared<WhileNode>(context->stmtNum);
    expect(TokenTypes::LEFT_PAREN, "(");
    CondExprParser condExprParser(context);
    std::shared_ptr<ExprNode> condExpr = condExprParser.parse();
    expect(TokenTypes::RIGHT_PAREN, ")");
    expect(TokenTypes::LEFT_BRACE, "{");
    context->stmtNum++;
    StmtListParser stmtListParser(context);
    std::shared_ptr<StmtListNode> stmtList = stmtListParser.parse();
    expect(TokenTypes::RIGHT_BRACE, "}");

    whileNode->addChild(condExpr);
    whileNode->addChild(stmtList);
    context->stmtNum--;

    return whileNode;
}