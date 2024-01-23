#include "WithClauseParser.h"

#include <map>

#include "qps/WithArgument.h"

WithClauseParser::WithClauseParser() : ConstraintClauseParser() {}

WithClauseParser::WithClauseParser(std::shared_ptr<SyntaxValidator> syntaxValidator) : ConstraintClauseParser(std::move(syntaxValidator)) {}

std::vector<std::shared_ptr<QueryToken>>::const_iterator WithClauseParser::parseClause(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    syntaxValidator->validateWithClause(++itr);

    allClauseInfo = {};
    itr = parseAttrCond(itr);

    return itr;
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator WithClauseParser::parseAttrCond(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    itr = parseAttrCompare(itr);

    while ((*(itr + 1))->getTokenString() == "and") {
        itr = parseAttrCompare(itr + 2);
    }

    return itr;
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator WithClauseParser::parseAttrCompare(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    // initialise before populating
    currentClauseInfo = {};
    currentClauseInfo.clauseType = "WithClause";

    itr = parseNot(itr);

    itr = parseArg(itr);
    // next ref is 2 tokens away
    itr = parseArg(itr + 2);
    allClauseInfo.emplace_back(currentClauseInfo);
    return itr;
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator WithClauseParser::parseArg(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    std::string argStr;
    std::shared_ptr<Attribute> attribute;
    std::string firstToken = (*itr)->getTokenString();

    if (firstToken == INVERTED_COMMA) {
        argStr = firstToken;
        argStr += (*(++itr))->getTokenString();
        argStr += (*(++itr))->getTokenString();
    } else {  // integer or synonym.attrRef
        argStr = firstToken;

        if (syntaxValidator->isName(firstToken)) {
            itr = itr + 2;
            attribute = parseAttrName((*itr)->getTokenString());
            if ((*(itr + 1))->getTokenString() == "#") {
                itr++;
            }
        }
    }

    ArgType argType = parseArgType(argStr);

    if (argType == SYNONYM) {
        currentClauseInfo.arguments.emplace_back(std::make_shared<WithArgument>(argType, argStr, attribute));
    } else {
        currentClauseInfo.arguments.emplace_back(std::make_shared<Argument>(argType, argStr));
    }

    return itr;
}

std::shared_ptr<Attribute> WithClauseParser::parseAttrName(const std::string& attrName) {
    std::map<std::string, AttrName> map = {
        {"procName", PROCNAME_ATTR},
        {"varName", VARNAME_ATTR},
        {"value", VALUE_ATTR},
        {"stmt", STMT_ATTR}};

    return std::make_shared<Attribute>(map[attrName]);
}

std::vector<clauseInfo> WithClauseParser::getClauseInfo() {
    return this->allClauseInfo;
}