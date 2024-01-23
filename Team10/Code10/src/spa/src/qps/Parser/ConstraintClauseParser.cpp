#include "ConstraintClauseParser.h"

#include <utility>

ConstraintClauseParser::ConstraintClauseParser(std::shared_ptr<SyntaxValidator> syntaxValidator) : syntaxValidator(std::move(syntaxValidator)) {}

bool ConstraintClauseParser::isArgTerminatingString(const std::string& endStr) {
    return endStr == "," || endStr == ")";
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator ConstraintClauseParser::parseArg(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    std::string argStr;
    std::string currentStr = (*itr)->getTokenString();
    int noOfOpenParentheses = 0;

    while (!isArgTerminatingString(currentStr) || noOfOpenParentheses != 0) {
        if ((*itr)->getTokenType() == END_OF_QUERY) {
            break;
        }

        if (currentStr != " ") {
            argStr += currentStr;
        }

        if (currentStr == OPEN_PARENTHESIS) {
            noOfOpenParentheses++;
        } else if (currentStr == CLOSE_PARENTHESIS) {
            noOfOpenParentheses--;
        }

        currentStr = (*(++itr))->getTokenString();
    }

    currentClauseInfo.arguments.emplace_back(std::make_shared<Argument>(parseArgType(argStr), argStr));

    return itr;
}

ArgType ConstraintClauseParser::parseArgType(const std::string& arg) {
    if (syntaxValidator->isName(arg)) {
        return SYNONYM;
    } else if (arg == "_") {
        return ArgType::WILDCARD;
    } else if (syntaxValidator->isInteger(arg)) {
        return INT_ARG;
    } else if (arg[0] == '\"' && syntaxValidator->isName(arg.substr(1, arg.length() - 2)) &&
               arg[arg.length() - 1] == '\"') {
        return STRING_ARG;
    } else {
        return UNKNOWN_ARG;
    }
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator ConstraintClauseParser::parseNot(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    if ((*itr)->getTokenString() == "not") {
        currentClauseInfo.isNot = true;
        itr++;
    } else {
        currentClauseInfo.isNot = false;
    }
    return itr;
}
