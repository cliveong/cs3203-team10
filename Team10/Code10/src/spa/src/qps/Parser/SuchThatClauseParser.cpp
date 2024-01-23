#include "SuchThatClauseParser.h"

#include <utility>
#include <vector>

#include "qps/Tokenizer/QueryToken.h"

SuchThatClauseParser::SuchThatClauseParser() : ConstraintClauseParser() {}

SuchThatClauseParser::SuchThatClauseParser(std::shared_ptr<SyntaxValidator> syntaxValidator) : ConstraintClauseParser(std::move(syntaxValidator)) {}

std::vector<std::shared_ptr<QueryToken>>::const_iterator SuchThatClauseParser::parseClause(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    if ((*itr)->getTokenString() != "such") {
        return --itr;
    } else {
        syntaxValidator->validateSuchThatClause(++itr);
    }

    allClauseInfo = {};
    itr = parseRelCond(++itr);

    return itr;
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator SuchThatClauseParser::parseRelCond(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    itr = parseRelRef(itr);

    while ((*(itr + 1))->getTokenString() == "and") {
        itr = parseRelRef(itr + 2);
    }

    return itr;
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator SuchThatClauseParser::parseRelRef(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    // initialise before populating
    currentClauseInfo = {};

    itr = parseNot(itr);

    std::string relationship = (*itr)->getTokenString();

    if ((*(++itr))->getTokenString() == STAR) {
        relationship += STAR;
        itr++;
    }

    currentClauseInfo.clauseType = relationship;

    // at this point, itr pointing at '('

    while ((*itr)->getTokenType() != END_OF_QUERY && (*itr)->getTokenString() != CLOSE_PARENTHESIS) {
        itr = parseArg(++itr);
    }

    syntaxValidator->validateArguments(relationship, currentClauseInfo.arguments);
    allClauseInfo.emplace_back(currentClauseInfo);

    return itr;
}

std::vector<clauseInfo> SuchThatClauseParser::getClauseInfo() {
    return this->allClauseInfo;
}
