#include "ExprParser.h"

#include "TermParser.h"
#include "commons/ExprNode.h"

ExprParser::ExprParser(std::shared_ptr<ParserContext> context)
    : Parser(context) {}

std::shared_ptr<ExprNode> ExprParser::parse() {
    TermParser termParser(context);
    std::shared_ptr<ExprNode> exprNode = termParser.parse();
    while (true) {
        Token opToken = getNextToken();
        if (opToken.type == TokenTypes::PLUS || opToken.type == TokenTypes::MINUS) {
            std::shared_ptr<ExprNode> opNode = std::make_shared<ExprNode>(
                opToken.value, opToken.value, context->stmtNum);
            opNode->addChild(exprNode);

            std::shared_ptr<ExprNode> rightOperand = termParser.parse();
            opNode->addChild(rightOperand);

            exprNode = opNode;
        } else if (opToken.type == TokenTypes::RIGHT_PAREN &&
                   context->parenCounter <= 0) {
            throw std::runtime_error("Error: Unexpected ')'");
        } else {
            context->currTokenIndex--;
            break;
        }
    }

    return exprNode;
}