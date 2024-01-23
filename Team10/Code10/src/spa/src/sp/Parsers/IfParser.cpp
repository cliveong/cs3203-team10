#include "IfParser.h"

#include "CondExprParser.h"
#include "StmtListParser.h"
#include "commons/Constants.h"

IfParser::IfParser(std::shared_ptr<ParserContext> context) : Parser(context) {}

std::shared_ptr<IfNode> IfParser::parse() {
    std::shared_ptr<IfNode> ifNode = std::make_shared<IfNode>(context->stmtNum);
    expect(TokenTypes::LEFT_PAREN, "(");
    CondExprParser condExprParser(context);
    std::shared_ptr<ExprNode> condExpr = condExprParser.parse();
    expect(TokenTypes::RIGHT_PAREN, ")");

    StmtListParser stmtListParser(context);
    expect(TokenTypes::THEN, "then");
    expect(TokenTypes::LEFT_BRACE, "{");
    context->stmtNum++;
    std::shared_ptr<StmtListNode> thenStmtList = stmtListParser.parse();
    thenStmtList->setValue(Constants::THEN_STMT_LIST);
    expect(TokenTypes::RIGHT_BRACE, "}");

    expect(TokenTypes::ELSE, "else");
    expect(TokenTypes::LEFT_BRACE, "{");
    std::shared_ptr<StmtListNode> elseStmtList = stmtListParser.parse();
    elseStmtList->setValue(Constants::ELSE_STMT_LIST);
    expect(TokenTypes::RIGHT_BRACE, "}");

    ifNode->addChild(condExpr);
    ifNode->addChild(thenStmtList);
    ifNode->addChild(elseStmtList);
    context->stmtNum--;

    return ifNode;
}