#include "PatternClauseParser.h"

#include <algorithm>
#include <utility>

#include "commons/ExprNode.h"
#include "qps/ExprArgument.h"
#include "qps/Synonym.h"

PatternClauseParser::PatternClauseParser() : ConstraintClauseParser() {}
PatternClauseParser::PatternClauseParser(std::shared_ptr<SyntaxValidator> syntaxValidator) : ConstraintClauseParser(std::move(syntaxValidator)) {}

std::vector<std::shared_ptr<QueryToken>>::const_iterator
PatternClauseParser::parseClause(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    if ((*itr)->getTokenString() != "pattern") {
        return --itr;
    }

    syntaxValidator->validatePatternClause(++itr);

    allClauseInfo = {};
    itr = parsePatternCond(itr);

    return itr;
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator PatternClauseParser::parsePatternCond(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    itr = parsePattern(itr);
    while ((*(itr + 1))->getTokenString() == "and") {
        itr = parsePattern(itr + 2);
    }
    return itr;
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator PatternClauseParser::parsePattern(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    // initialise before populating
    currentClauseInfo = {};
    exprTokens = {};

    itr = parseNot(itr);

    currentSynonymName = (*itr)->getTokenString();

    // first arg starts 2 tokens away: 'synonym', '('
    itr = parseArg(itr + 2);

    // second arg
    auto secondCommaItr = parseArg(++itr);
    if (currentClauseInfo.arguments[1]->getValue() != WILDCARD) {
        itr = storeExprTokens(itr);
        currentClauseInfo.arguments[1]->setArgType(EXPRESSION);
    } else {
        if ((*(++itr))->getTokenString() == COMMA) {  // third arg
            itr = parseArg(++secondCommaItr);
        } else {  // second arg
            itr = secondCommaItr;
        }
    }

    patternClauseInfo patternClauseInfo;
    if (currentClauseInfo.arguments[1]->getType() == EXPRESSION) {
        currentClauseInfo.arguments[1] = std::make_shared<ExprArgument>(EXPRESSION, currentClauseInfo.arguments[1]->getValue(),
                                                                        buildPatternAST(0, exprTokens.size() - 1));
    }

    if (currentClauseInfo.arguments.size() == 3) {
        currentClauseInfo.clauseType = "IfPattern";
    } else {
        if (currentClauseInfo.arguments[1]->getType() == EXPRESSION) {
            currentClauseInfo.clauseType = "AssignPattern";
        } else {
            currentClauseInfo.clauseType = "Pattern";
        }
    }

    patternClauseInfo.clauseInfo = currentClauseInfo;
    patternClauseInfo.synonymName = currentSynonymName;

    syntaxValidator->validateArguments(currentClauseInfo.clauseType, currentClauseInfo.arguments);
    allClauseInfo.emplace_back(patternClauseInfo);

    return itr;
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator PatternClauseParser::storeExprTokens(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    int noOfOpenParentheses = 0;

    std::string currentTokenStr = (*itr)->getTokenString();

    while (currentTokenStr != CLOSE_PARENTHESIS || noOfOpenParentheses != 0) {
        if (currentTokenStr == OPEN_PARENTHESIS) {
            noOfOpenParentheses++;
        } else if (currentTokenStr == CLOSE_PARENTHESIS) {
            noOfOpenParentheses--;
        } else if (currentTokenStr == SPACE) {
            currentTokenStr = (*(++itr))->getTokenString();
            continue;
        }

        if (currentTokenStr != WILDCARD && currentTokenStr != INVERTED_COMMA) {
            exprTokens.emplace_back(*itr);
        }

        currentTokenStr = (*(++itr))->getTokenString();
    }

    return itr;
}

std::shared_ptr<ExprNode> PatternClauseParser::buildPatternAST(int startIndex, int endIndex) {
    std::string VAR = "var";
    std::string CONST = "const";

    // One single token
    if (startIndex == endIndex) {
        std::string nodeValue = (exprTokens[startIndex])->getTokenString();
        if (syntaxValidator->isName(nodeValue)) {
            return std::make_shared<ExprNode>(VAR, nodeValue, 1);
        } else if (syntaxValidator->isInteger(nodeValue)) {
            return std::make_shared<ExprNode>(CONST, nodeValue, 1);
        }
    }

    int currIndex = startIndex;
    std::string currentTokenStr = (exprTokens[currIndex])->getTokenString();

    // Expression wrapped in brackets
    if (currentTokenStr == OPEN_PARENTHESIS && findCloseParenthesis(startIndex + 1, endIndex) == endIndex) {
        return buildPatternAST(startIndex + 1, endIndex - 1);
    }

    // find the index of root node
    int rootIndex = findRootIndex(startIndex, endIndex);

    // create root and child nodes
    std::string nodeValue = (exprTokens[rootIndex])->getTokenString();
    std::shared_ptr<ExprNode> rootNode = std::make_shared<ExprNode>(nodeValue, nodeValue, 1);
    rootNode->addChild(buildPatternAST(startIndex, rootIndex - 1));
    rootNode->addChild(buildPatternAST(rootIndex + 1, endIndex));

    return rootNode;
}

int PatternClauseParser::findRootIndex(int startIndex, int endIndex) {
    int noOfOpenParentheses = 0;

    int exprOprIndex = -1;
    int termOprIndex = -1;

    int currIndex = startIndex;
    while (currIndex <= endIndex) {
        std::string currentTokenStr = (exprTokens[currIndex])->getTokenString();
        if (currentTokenStr == OPEN_PARENTHESIS) {
            noOfOpenParentheses++;
        } else if (currentTokenStr == CLOSE_PARENTHESIS) {
            noOfOpenParentheses--;
        }

        if (noOfOpenParentheses != 0) {
            currIndex++;
            continue;
        }

        if (isExprOperator(currentTokenStr)) {
            exprOprIndex = currIndex;
        } else if (isTermOperator(currentTokenStr)) {
            termOprIndex = currIndex;
        }

        currIndex++;
    }

    if (exprOprIndex != -1) {
        return exprOprIndex;
    } else {  // either exprOprIndex or termOprIndex is guaranteed to exist
        return termOprIndex;
    }
}

int PatternClauseParser::findCloseParenthesis(int startIndex, int endIndex) {
    int noOfOpenParenthesis = 0;
    int currIndex = startIndex;

    std::string currentTokenStr;
    while (currIndex <= endIndex) {
        currentTokenStr = (exprTokens[currIndex])->getTokenString();
        if (currentTokenStr == CLOSE_PARENTHESIS) {
            if (noOfOpenParenthesis == 0) {
                break;
            } else {
                noOfOpenParenthesis--;
            }

        } else if (currentTokenStr == OPEN_PARENTHESIS) {
            noOfOpenParenthesis++;
        }
        currIndex++;
    }
    return currIndex;
}

bool PatternClauseParser::isExprOperator(std::string str) {
    return str == "+" || str == "-";
}

bool PatternClauseParser::isTermOperator(std::string str) {
    return str == "*" || str == "/" || str == "%";
}

std::vector<struct patternClauseInfo> PatternClauseParser::getClauseInfo() {
    return this->allClauseInfo;
}