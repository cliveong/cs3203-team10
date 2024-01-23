#include "ProcedureParser.h"

#include "../../commons/Constants.h"
#include "StmtListParser.h"

ProcedureParser::ProcedureParser(std::shared_ptr<ParserContext> context)
    : Parser(context) {}

std::shared_ptr<ProcedureNode> ProcedureParser::parse() {
    expect(TokenTypes::PROCEDURE, "procedure");
    Token nameToken = expect(TokenTypes::NAME, "");
    std::shared_ptr<ProcedureNode> procedureNode =
        std::make_shared<ProcedureNode>(nameToken.value);
    expect(TokenTypes::LEFT_BRACE, "{");
    StmtListParser stmtListParser(context);
    std::shared_ptr<StmtListNode> stmtListNode = stmtListParser.parse();

    if (stmtListNode->getChildren().size() > 0) {
        procedureNode->addChild(stmtListNode);
    }

    expect(TokenTypes::RIGHT_BRACE, "}");
    return procedureNode;
}