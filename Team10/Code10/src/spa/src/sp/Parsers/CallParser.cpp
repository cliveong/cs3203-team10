#include "CallParser.h"

#include "../../commons/Constants.h"
#include "ProgramParser.h"

CallParser::CallParser(std::shared_ptr<ParserContext> context)
    : Parser(context) {}

std::shared_ptr<CallNode> CallParser::parse() {
    Token procNameToken = expect(TokenTypes::NAME, "");
    const std::string procName = procNameToken.value;

    std::shared_ptr<CallNode> node = std::make_shared<CallNode>(procName, context->stmtNum);
    expect(TokenTypes::SEMICOLON, ";");

    return node;
}