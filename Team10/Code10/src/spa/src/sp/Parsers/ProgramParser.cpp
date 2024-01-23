#include "ProgramParser.h"

#include <unordered_set>

#include "ProcedureParser.h"

ProgramParser::ProgramParser(std::shared_ptr<ParserContext> context)
    : Parser(context) {}

std::shared_ptr<ProgramNode> ProgramParser::parse() {
    std::shared_ptr<ProgramNode> programNode = std::make_shared<ProgramNode>();
    std::unordered_set<std::string> procNames;

    while (true) {
        Token nextToken = getNextToken();
        if (nextToken.type == TokenTypes::PROCEDURE) {
            context->currTokenIndex--;
            ProcedureParser procedureParser(context);
            std::shared_ptr<ProcedureNode> procNode = procedureParser.parse();
            programNode->addChild(procNode);

            if (procNames.find(procNode->getValue()) != procNames.end()) {
                throw std::runtime_error("Error: Duplicate procedure names");
            } else {
                procNames.insert(procNode->getValue());
            }
        } else {
            if (nextToken.type != TokenTypes::EOF_TOKEN) {
                throw std::runtime_error("Error: Unexpected '" + nextToken.value + "'");
            }
            context->currTokenIndex--;
            expect(TokenTypes::EOF_TOKEN, "");
            break;
        }
    }

    if (programNode->getChildren().size() == 0) {
        throw std::runtime_error("Error: Invalid program");
    }

    return programNode;
}