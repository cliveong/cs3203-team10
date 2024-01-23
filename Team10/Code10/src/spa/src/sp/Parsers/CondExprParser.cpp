#include "CondExprParser.h"

#include "RelExprParser.h"

CondExprParser::CondExprParser(std::shared_ptr<ParserContext> context)
    : Parser(context) {}

std::shared_ptr<ExprNode> CondExprParser::parse() {
    Token token = getNextToken();
    RelExprParser relExprParser(context);

    if (token.type == TokenTypes::NOT) {
        return validateNotExpr();
    } else if (token.type == TokenTypes::LEFT_PAREN) {
        int saveIndex = context->currTokenIndex;
        try {
            context->currTokenIndex--;
            saveIndex = context->currTokenIndex;
            return validateParenExpr();
        } catch (const std::runtime_error& e) {
            context->currTokenIndex = saveIndex;
            return relExprParser.parse();
        }
    } else {
        context->currTokenIndex--;
        return relExprParser.parse();
    }
}

std::shared_ptr<ExprNode> CondExprParser::validateNotExpr() {
    expect(TokenTypes::LEFT_PAREN, "(");
    std::shared_ptr<ExprNode> condExprNode = parse();
    expect(TokenTypes::RIGHT_PAREN, ")");
    std::shared_ptr<ExprNode> notNode =
        std::make_shared<ExprNode>("!", "!", context->stmtNum);
    notNode->addChild(condExprNode);
    return notNode;
}

std::shared_ptr<ExprNode> CondExprParser::validateParenExpr() {
    expect(TokenTypes::LEFT_PAREN, "(");
    std::shared_ptr<ExprNode> left = parse();
    expect(TokenTypes::RIGHT_PAREN, ")");
    while (true) {
        Token opToken = getNextToken();
        if (opToken.type == TokenTypes::AND || opToken.type == TokenTypes::OR) {
            std::shared_ptr<ExprNode> opNode = std::make_shared<ExprNode>(
                opToken.value, opToken.value, context->stmtNum);
            opNode->addChild(left);
            expect(TokenTypes::LEFT_PAREN, "(");
            std::shared_ptr<ExprNode> right = parse();

            opNode->addChild(right);
            left = opNode;

            if (getNextToken().type != TokenTypes::RIGHT_PAREN) {
                context->currTokenIndex--;
                break;
            }
        } else {
            context->currTokenIndex--;
            return left;
        }
    }
    return left;
}