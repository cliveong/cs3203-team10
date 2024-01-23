#include "RelFactorParser.h"

#include "ExprParser.h"

RelFactorParser::RelFactorParser(std::shared_ptr<ParserContext> context)
    : Parser(context) {}

std::shared_ptr<ExprNode> RelFactorParser::parse() {
    ExprParser exprParser(context);
    std::shared_ptr<ExprNode> exprNode = exprParser.parse();
    return exprNode;
}