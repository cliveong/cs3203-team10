#include "SyntaxValidator.h"

#include <string>

#include "qps/Validator/ValidatorErrors/SyntaxException.h"

bool SyntaxValidator::isName(const std::string& name) {
    if (name.empty()) {
        return false;
    }

    bool isLetterFirst = isalpha(name[0]);
    if (name.length() > 1) {
        for (char letter : name.substr(1, name.length() - 1)) {
            if (!iswalnum(letter)) {
                return false;
            }
        }
    }
    return isLetterFirst;
}

bool SyntaxValidator::isInteger(const std::string& integer) {
    if (integer.empty()) {
        return false;
    }

    if (integer[0] == '0') {
        return integer.length() == 1;
    } else {
        for (int i = 0; i < integer.length(); i++) {
            if (!isdigit(integer[i])) {
                return false;
            }
        }
        return true;
    }
}

bool SyntaxValidator::isOperator(const std::string& opr) {
    return opr == "+" || opr == "-" || opr == "*" || opr == "/" || opr == "%";
}

bool SyntaxValidator::isFactor(const std::string& factor) {
    return isName(factor) || isInteger(factor);
}

bool SyntaxValidator::isOpenParenthesis(const std::string& openParenthesis) {
    return openParenthesis == "(";
}

bool SyntaxValidator::isComma(const std::string& comma) {
    return comma == ",";
}

bool SyntaxValidator::isCloseParenthesis(const std::string& closeParenthesis) {
    return closeParenthesis == ")";
}

bool SyntaxValidator::isSemiColon(const std::string& semicolon) {
    return semicolon == ";";
}

bool SyntaxValidator::isInvertedComma(const std::string& invertedComma) {
    return invertedComma == "\"";
}

bool SyntaxValidator::isWildcard(const std::string& wildcard) {
    return wildcard == "_";
}

bool SyntaxValidator::isSpace(const std::string& space) {
    return space == " ";
}

bool SyntaxValidator::isStar(const std::string& star) {
    return star == "*";
}

bool SyntaxValidator::isEqualSign(const std::string& equal) {
    return equal == "=";
}

bool SyntaxValidator::isDot(const std::string& dot) {
    return dot == ".";
}

bool SyntaxValidator::isOpenAnglularParenthesis(const std::string& angularParenthesis) {
    return angularParenthesis == "<";
}

bool SyntaxValidator::isCloseAnglularParenthesis(const std::string& angularParenthesis) {
    return angularParenthesis == ">";
}

bool SyntaxValidator::validateSynonym(const std::string& synonymName) {
    if (isName(synonymName)) {
        return true;
    } else {
        throw SyntaxException("Syntax Error: synonym name is wrong!");
    }
}

void SyntaxValidator::validateDeclaration(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    std::string synonymName = (*itr)->getTokenString();
    validateSynonym(synonymName);

    std::string nextTokenString = (*(++itr))->getTokenString();
    if (isSemiColon(nextTokenString)) {
        return;
    } else if (isComma(nextTokenString)) {
        validateDeclaration(++itr);
    } else {
        throw SyntaxException("Syntax Error: Declaration syntax is wrong!");
    }
}

void SyntaxValidator::validateSelectClause(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    std::string tokenString = (*itr)->getTokenString();
    if (tokenString == "Select") {
        if ((*(++itr))->getTokenType() == END_OF_QUERY) {
            throw SyntaxException("Syntax Error: No synonym after select!");
        }

        std::string currentToken = (*itr)->getTokenString();
        if ((*itr)->getTokenString() != "BOOLEAN") {
            validateTuple(itr);
        }

    } else {
        throw SyntaxException("Select word should be after declaration line!");
    }
}

void SyntaxValidator::validateTuple(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    std::string firstToken = (*itr)->getTokenString();

    if (isOpenAnglularParenthesis(firstToken)) {
        itr = validateElem(++itr);

        while (isComma((*(++itr))->getTokenString())) {
            itr = validateElem(++itr);
        }
        if (!isCloseAnglularParenthesis((*itr)->getTokenString())) {
            throw SyntaxException("Syntax Error: Missing '>'!");
        }
    } else {
        itr = validateElem(itr);
    }
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator SyntaxValidator::validateElem(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    validateSynonym((*itr)->getTokenString());

    if (isDot((*(itr + 1))->getTokenString())) {
        itr = validateAttrName(itr + 2);
    }

    return itr;
}

void SyntaxValidator::validateSuchThatClause(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    if ((*(itr))->getTokenString() != "that") {
        throw SyntaxException(R"(Syntax Error: 'that' should be after 'such'!)");
    } else {
        itr++;
    }

    validateRelCond(itr);
}

void SyntaxValidator::validateRelCond(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    itr = validateRelRef(itr);
    while ((*(itr + 1))->getTokenString() == "and") {
        itr = validateRelRef(itr + 2);
    }
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator SyntaxValidator::validateRelRef(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    if ((*itr)->getTokenType() == END_OF_QUERY) {
        throw SyntaxException("Syntax Error: Missing relRef!");
    }

    if ((*itr)->getTokenString() == "not") {
        itr++;
    }

    std::string relRef = (*itr)->getTokenString();

    if (isStar((*(++itr))->getTokenString())) {
        relRef += "*";
        itr++;
    }

    if (relRefList.find(relRef) == relRefList.end()) {
        throw SyntaxException("Syntax Error: invalid relationship!");
    }

    if (!isOpenParenthesis((*itr)->getTokenString())) {
        throw SyntaxException(R"(Syntax Error: missing '('!)");
    }

    itr = validateArgRef(++itr, ",");
    itr = validateArgRef(++itr, ")");
    return itr;
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator SyntaxValidator::validateArgRef(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr, const std::string& endString) {
    if ((*itr)->getTokenType() == END_OF_QUERY) {
        throw SyntaxException("Syntax Error: Invalid Argument!");
    }

    auto currentTokenStr = (*itr)->getTokenString();

    if (isInvertedComma(currentTokenStr)) {
        if (!isName((*(++itr))->getTokenString())) {
            throw SyntaxException("Syntax Error: Invalid Argument!");
        }
        if (!isInvertedComma((*(++itr))->getTokenString())) {
            throw SyntaxException("Syntax Error: Invalid Argument!");
        }

    } else if (!isName(currentTokenStr) && !isInteger(currentTokenStr) && !isWildcard(currentTokenStr)) {
        throw SyntaxException("Syntax Error: Invalid Argument!");
    }

    if ((*(++itr))->getTokenString() != endString) {
        auto errorMsg = "Syntax Error: Missing " + endString + "!";
        throw SyntaxException(errorMsg);
    }
    return itr;
}

void SyntaxValidator::validatePatternClause(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    if ((*itr)->getTokenType() == END_OF_QUERY) {
        throw SyntaxException("Syntax Error: Missing pattern!");
    }

    if ((*itr)->getTokenString() == "not") {
        itr++;
    }

    validateSynonym((*(itr))->getTokenString());

    if (!isOpenParenthesis((*(++itr))->getTokenString())) {
        throw SyntaxException(R"(Syntax Error: missing '('!)");
    }

    itr = validateArgRef(++itr, ",");

    itr = validatePatternSecondArg(++itr);

    if ((*(++itr))->getTokenString() == "and") {
        validatePatternClause(++itr);
    }
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator SyntaxValidator::validatePatternSecondArg(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    if ((*itr)->getTokenType() == END_OF_QUERY) {
        throw SyntaxException("Syntax Error: Missing second arg!");
    }

    // first token of second arg
    std::string currentTokenStr = (*itr)->getTokenString();

    if (isWildcard(currentTokenStr)) {
        // partial match: _"..."_
        if (isInvertedComma((*(itr + 1))->getTokenString())) {
            itr = validateExpr(itr + 2);
            if (!isInvertedComma((*itr)->getTokenString())) {
                throw SyntaxException(R"(Syntax Error: Missing '"' for partial match!)");
            }
            if (!isWildcard((*(++itr))->getTokenString())) {
                throw SyntaxException(R"(Syntax Error: Missing '_' for partial match!)");
            }
            if (!isCloseParenthesis((*(++itr))->getTokenString())) {
                throw SyntaxException(R"(Syntax Error: Missing ')'!)");
            }
        } else {
            itr = validateWildcardsAfterFirstArg(++itr);
        }
    } else if (isInvertedComma(currentTokenStr)) {  // exact match: "..."
        itr = validateExpr(++itr);
        if (!isInvertedComma((*itr)->getTokenString())) {
            throw SyntaxException(R"(Syntax Error: Missing '"' for partial match!)");
        }
        if (!isCloseParenthesis((*(++itr))->getTokenString())) {
            throw SyntaxException(R"(Syntax Error: Missing ')'!)");
        }
    } else {
        throw SyntaxException(R"(Syntax Error: Invalid second arg!)");
    }

    return itr;
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator SyntaxValidator::validateWildcardsAfterFirstArg(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    if ((*itr)->getTokenType() == END_OF_QUERY) {
        throw SyntaxException("Syntax Error: Missing second arg!");
    }

    std::string currentTokenStr = (*(itr))->getTokenString();

    if (isCloseParenthesis(currentTokenStr)) {  // only two args
        return itr;
    } else if (isComma(currentTokenStr)) {  // third arg is present: if-clause
        if (!isWildcard((*(++itr))->getTokenString())) {
            throw SyntaxException(R"(Syntax Error: missing '_'!)");
        }

        if (!isCloseParenthesis((*(++itr))->getTokenString())) {
            throw SyntaxException(R"(Syntax Error: missing ')'!)");
        }

    } else {
        throw SyntaxException(R"(Syntax Error: Wrong symbol after second '_'!)");
    }

    return itr;
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator SyntaxValidator::validateExpr(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    std::string currentTokenStr = (*(itr))->getTokenString();

    int noOfOpenParentheses = 0;

    // check first token in expression: can only be ( or Factor
    if (!isOpenParenthesis(currentTokenStr) && !isFactor(currentTokenStr)) {
        throw SyntaxException("Syntax Error: wrong pattern expression!");
    }

    while (!isInvertedComma(currentTokenStr)) {
        if ((*itr)->getTokenType() == END_OF_QUERY) {
            throw SyntaxException("Syntax Error: wrong pattern expression!");
        }

        if (isOpenParenthesis(currentTokenStr)) {
            validateTokenAfterOpenParenthesis(itr);
            noOfOpenParentheses++;
        } else if (isCloseParenthesis(currentTokenStr)) {
            validateTokenAfterCloseParenthesis(itr);
            noOfOpenParentheses--;
        } else if (isOperator(currentTokenStr)) {
            validateTokenAfterOperator(itr);
        } else if (isSpace(currentTokenStr)) {
        } else if (isFactor(currentTokenStr)) {
            validateTokenAfterFactor(itr);
        } else {
            throw SyntaxException("Syntax Error: wrong pattern expression!");
        }
        currentTokenStr = (*(++itr))->getTokenString();
    }

    if (noOfOpenParentheses != 0) {
        throw SyntaxException("Syntax Error: Missing bracket!");
    }

    return itr;
}

void SyntaxValidator::validateTokenAfterOpenParenthesis(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    std::string nextTokenStr = (*(itr + 1))->getTokenString();
    if (!isOpenParenthesis(nextTokenStr) && !isFactor(nextTokenStr)) {
        throw SyntaxException("Syntax Error: wrong pattern expression!");
    }
}

void SyntaxValidator::validateTokenAfterCloseParenthesis(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    std::string nextTokenStr = (*(itr + 1))->getTokenString();
    if (!isCloseParenthesis(nextTokenStr) && !isOperator(nextTokenStr) && !isSpace(nextTokenStr) && !isInvertedComma(nextTokenStr)) {
        throw SyntaxException("Syntax Error: wrong pattern expression!");
    }
}

void SyntaxValidator::validateTokenAfterOperator(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    std::string nextTokenStr = (*(itr + 1))->getTokenString();
    if (!isFactor(nextTokenStr) && !isOpenParenthesis(nextTokenStr)) {
        throw SyntaxException("Syntax Error: wrong pattern expression!");
    }
}

void SyntaxValidator::validateTokenAfterFactor(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    std::string nextTokenStr = (*(itr + 1))->getTokenString();
    if (!isOperator(nextTokenStr) && !isCloseParenthesis(nextTokenStr) && !isSpace(nextTokenStr) && !isInvertedComma(nextTokenStr)) {
        throw SyntaxException("Syntax Error: wrong pattern expression!");
    }
}

void SyntaxValidator::validateArguments(const std::string& clauseType, std::vector<std::shared_ptr<Argument>> arguments) {
    if (clauseType == "AssignPattern" || clauseType == "WhilePattern" || clauseType == "IfPattern" || clauseType == "Pattern") {
        if (arguments.size() != 2 && arguments.size() != 3) {
            throw SyntaxException("Syntax Error: Wrong number of arguments!");
        }
    } else {
        if (arguments.size() != 2) {
            throw SyntaxException("Syntax Error: Wrong number of arguments!");
        }
    }

    std::shared_ptr<Argument> firstArg = arguments[0];
    std::shared_ptr<Argument> secondArg = arguments[1];

    if (clauseType == "AssignPattern" || clauseType == "WhilePattern" || clauseType == "IfPattern" || clauseType == "Pattern") {
        validateEntRef(firstArg->getType());
    } else if (clauseType == "Uses" || clauseType == "Modifies") {
        if (firstArg->getType() != STRING_ARG) {
            validateStmtRef(firstArg->getType());
        }
        validateEntRef(secondArg->getType());
    } else if (clauseType == "Calls" || clauseType == "Calls*") {
        validateEntRef(firstArg->getType());
        validateEntRef(secondArg->getType());
    } else {  // Parent/Parent*/Follows/Follows*
        validateStmtRef(firstArg->getType());
        validateStmtRef(secondArg->getType());
    }
}

void SyntaxValidator::validateStmtRef(const ArgType& argumentType) {
    if (argumentType != SYNONYM && argumentType != WILDCARD && argumentType != INT_ARG) {
        throw SyntaxException("Syntax Error: Wrong entRef!");
    }
}

void SyntaxValidator::validateEntRef(const ArgType& argumentType) {
    if (argumentType != SYNONYM && argumentType != WILDCARD && argumentType != STRING_ARG) {
        throw SyntaxException("Syntax Error: Wrong entRef!");
    }
}

void SyntaxValidator::validateWithClause(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    if ((*itr)->getTokenString() == "not") {
        itr++;
    }

    itr = validateRef(itr);
    if (!isEqualSign((*(++itr))->getTokenString())) {
        throw SyntaxException("Syntax Error: Missing '=' !");
    }
    validateRef(++itr);
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator SyntaxValidator::validateRef(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    if ((*itr)->getTokenType() == END_OF_QUERY) {
        throw SyntaxException("Syntax Error: Missing ref!");
    }

    std::string firstTokenStr = (*itr)->getTokenString();

    if (isInvertedComma(firstTokenStr)) {
        if (!isName((*(++itr))->getTokenString())) {
            throw SyntaxException("Syntax Error: Invalid string for ref!");
        }

        if (!isInvertedComma((*(++itr))->getTokenString())) {
            throw SyntaxException("Syntax Error: Missing closing '\"' for ref!");
        }
    } else if (isInteger(firstTokenStr)) {
    } else if (isName(firstTokenStr)) {
        if (!isDot((*(++itr))->getTokenString())) {
            throw SyntaxException("Syntax Error: Missing '.' after synonym for ref!");
        }

        itr = validateAttrName(++itr);
    } else {
        throw SyntaxException("Syntax Error: Invalid ref!");
    }
    return itr;
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator SyntaxValidator::validateAttrName(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    std::string attrName = (*itr)->getTokenString();
    if (attrNameList.find(attrName) == attrNameList.end()) {
        throw SyntaxException("Syntax Error: Invalid attrName!");
    }

    if (attrName == "stmt" && (*(++itr))->getTokenString() != "#") {
        throw SyntaxException("Syntax Error: Invalid attrName!");
    }
    return itr;
}

void SyntaxValidator::validateEndOfQuery(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    if ((*(itr))->getTokenType() != END_OF_QUERY) {
        throw SyntaxException("Syntax Error: Invalid Query!");
    }
}