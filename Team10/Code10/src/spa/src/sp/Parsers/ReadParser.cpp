#include "ReadParser.h"

#include "../../commons/Constants.h"

ReadParser::ReadParser(std::shared_ptr<ParserContext> context)
    : Parser(context) {}

std::shared_ptr<ReadNode> ReadParser::parse() {
    Token varToken = expect(TokenTypes::NAME, "");

    std::shared_ptr<ReadNode> node = std::make_shared<ReadNode>(context->stmtNum);
    node->addChild(std::make_shared<VarNode>(varToken.value, context->stmtNum));

    expect(TokenTypes::SEMICOLON, ";");

    return node;
}