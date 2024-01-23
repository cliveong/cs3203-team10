#include "StmtParser.h"

#include "AssignParser.h"
#include "CallParser.h"
#include "IfParser.h"
#include "PrintParser.h"
#include "ReadParser.h"
#include "WhileParser.h"

StmtParser::StmtParser(std::shared_ptr<ParserContext> context)
    : Parser(context) {}

std::shared_ptr<StmtNode> StmtParser::parse() {
    Token stmtToken = getNextToken();
    AssignParser assignParser(context, stmtToken);
    std::shared_ptr<StmtNode> stmtNode = std::make_shared<StmtNode>();

    if (stmtToken.type == TokenTypes::NAME) {
        stmtNode->addChild(assignParser.parse());
        return stmtNode;
    }

    if (!isKeyword(stmtToken.type)) {
        throw std::runtime_error("Error: Invalid statement");
    }

    int saveIndex = context->currTokenIndex;
    try {
        if (stmtToken.type == TokenTypes::READ) {
            ReadParser readParser(context);
            stmtNode->addChild(readParser.parse());
        } else if (stmtToken.type == TokenTypes::PRINT) {
            PrintParser printParser(context);
            stmtNode->addChild(printParser.parse());
        } else if (stmtToken.type == TokenTypes::IF) {
            IfParser ifParser(context);
            stmtNode->addChild(ifParser.parse());
        } else if (stmtToken.type == TokenTypes::WHILE) {
            WhileParser whileParser(context);
            stmtNode->addChild(whileParser.parse());
        } else if (stmtToken.type == TokenTypes::CALL) {
            CallParser callParser(context);
            stmtNode->addChild(callParser.parse());
        } else {
            stmtNode->addChild(assignParser.parse());
        }
        return stmtNode;

    } catch (const std::runtime_error& e) {
        context->currTokenIndex = saveIndex;
        stmtNode->addChild(assignParser.parse());
        return stmtNode;
    }
}