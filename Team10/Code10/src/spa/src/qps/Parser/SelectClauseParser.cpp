#include "SelectClauseParser.h"

SelectClauseParser::SelectClauseParser(std::shared_ptr<SyntaxValidator> syntaxValidator) : syntaxValidator(syntaxValidator) {}

std::vector<std::shared_ptr<QueryToken>>::const_iterator SelectClauseParser::parse(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    syntaxValidator->validateSelectClause(itr);

    itr = parseResultCl(++itr);

    return itr;
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator SelectClauseParser::parseResultCl(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    std::string currentToken = (*itr)->getTokenString();

    if (currentToken == "BOOLEAN") {
        this->returnBool = true;
        parseElem(itr);
    } else {
        this->returnBool = false;
        itr = parseTuple(itr);
    }

    return itr;
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator SelectClauseParser::parseTuple(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    std::string currentToken = (*itr)->getTokenString();

    if (currentToken == "<") {
        itr = parseElem(++itr);
        while ((*(++itr))->getTokenString() == ",") {
            itr = parseElem(++itr);
        }
    } else {
        itr = parseElem(itr);
    }
    return itr;
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator SelectClauseParser::parseElem(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    struct elemInfo currentElemInfo;
    currentElemInfo.synonymName = (*itr)->getTokenString();
    if ((*(itr + 1))->getTokenString() == ".") {
        itr++;
        currentElemInfo.attrName = (*(++itr))->getTokenString();
        if (currentElemInfo.attrName == "stmt") {
            itr++;  // Account for #
        }
    }
    this->allElemInfo.emplace_back(currentElemInfo);
    return itr;
}

std::vector<elemInfo> SelectClauseParser::getAllElemInfo() {
    return this->allElemInfo;
}

bool SelectClauseParser::isReturnBool() {
    return this->returnBool;
}