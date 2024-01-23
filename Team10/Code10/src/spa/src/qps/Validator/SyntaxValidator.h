#pragma once

#include <array>
#include <unordered_set>
#include <vector>

#include "qps/Argument.h"
#include "qps/Tokenizer/QueryToken.h"
#include "qps/Validator/ValidatorErrors/SyntaxException.h"

class SyntaxValidator {
   private:
    std::unordered_set<std::string> relRefList = {
        "Follows", "Follows*", "Parent", "Parent*", "Uses", "Modifies", "Calls", "Calls*", "Affects", "Next", "Next*"};
    std::unordered_set<std::string> attrNameList = {
        "procName", "varName", "value", "stmt"};

   public:
    bool isName(const std::string& name);
    bool isInteger(const std::string& integer);
    bool isOperator(const std::string& opr);
    bool isFactor(const std::string& factor);
    bool isOpenParenthesis(const std::string& openParenthesis);
    bool isComma(const std::string& comma);
    bool isCloseParenthesis(const std::string& closeParenthesis);
    bool isSemiColon(const std::string& semicolon);
    bool isInvertedComma(const std::string& invertedComma);
    bool isWildcard(const std::string& wildcard);
    bool isSpace(const std::string& space);
    bool isStar(const std::string& star);
    bool isEqualSign(const std::string& equal);
    bool isDot(const std::string& dot);
    bool isOpenAnglularParenthesis(const std::string& angularParenthesis);
    bool isCloseAnglularParenthesis(const std::string& angularParenthesis);

    bool validateSynonym(const std::string& synonymName);
    void validateDeclaration(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    void validateSelectClause(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    void validateTuple(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    std::vector<std::shared_ptr<QueryToken>>::const_iterator validateElem(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);

    void validateSuchThatClause(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    void validateRelCond(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    std::vector<std::shared_ptr<QueryToken>>::const_iterator validateRelRef(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    // checks for invalid tokens in arguments (stmtRef/entRef)
    std::vector<std::shared_ptr<QueryToken>>::const_iterator validateArgRef(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr, const std::string& endString);

    void validatePatternClause(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    std::vector<std::shared_ptr<QueryToken>>::const_iterator validatePatternSecondArg(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    // validates that arguments after the first arg is a wildcard
    std::vector<std::shared_ptr<QueryToken>>::const_iterator validateWildcardsAfterFirstArg(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    std::vector<std::shared_ptr<QueryToken>>::const_iterator validateExpr(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    void validateTokenAfterOpenParenthesis(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    void validateTokenAfterCloseParenthesis(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    void validateTokenAfterOperator(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    void validateTokenAfterFactor(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);

    void validateArguments(const std::string& clauseType, std::vector<std::shared_ptr<Argument>> arguments);
    void validateStmtRef(const ArgType& argumentType);
    void validateEntRef(const ArgType& argumentType);

    void validateWithClause(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    std::vector<std::shared_ptr<QueryToken>>::const_iterator validateRef(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    std::vector<std::shared_ptr<QueryToken>>::const_iterator validateAttrName(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    void validateEndOfQuery(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
};
