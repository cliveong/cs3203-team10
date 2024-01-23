#include "PrintParser.h"

#include "../../commons/Constants.h"

PrintParser::PrintParser(std::shared_ptr<ParserContext> context)
    : Parser(context) {}

std::shared_ptr<PrintNode> PrintParser::parse() {
    Token varToken = expect(TokenTypes::NAME, "");

    std::shared_ptr<PrintNode> node =
        std::make_shared<PrintNode>(context->stmtNum);
    node->addChild(std::make_shared<VarNode>(varToken.value, context->stmtNum));

    expect(TokenTypes::SEMICOLON, ";");

    return node;
}