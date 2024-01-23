#include "StmtListParser.h"

#include "../../commons/Constants.h"
#include "StmtParser.h"

StmtListParser::StmtListParser(std::shared_ptr<ParserContext> context)
    : Parser(context) {}

std::shared_ptr<StmtListNode> StmtListParser::parse() {
    std::shared_ptr<StmtListNode> stmtListNode = std::make_shared<StmtListNode>();

    while (true) {
        Token stmtToken = getNextToken();
        if (stmtToken.type == TokenTypes::RIGHT_BRACE ||
            stmtToken.type == TokenTypes::EOF_TOKEN) {
            context->currTokenIndex--;
            break;
        } else {
            StmtParser stmtParser(context);
            context->currTokenIndex--;
            std::shared_ptr<StmtNode> stmtNode = stmtParser.parse();
            stmtListNode->addChild(stmtNode->getChildren()[0]);
            context->stmtNum++;
        }
    }

    if (stmtListNode->getChildren().size() == 0) {
        throw std::runtime_error("Error: Statement list cannot be empty");
    }

    return stmtListNode;
}