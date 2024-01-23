#pragma once

#include "ConstraintClauseParser.h"
#include "commons/ExprNode.h"
#include "qps/Validator/SyntaxValidator.h"

struct patternClauseInfo {
    clauseInfo clauseInfo;
    std::string synonymName;
};

class PatternClauseParser : public ConstraintClauseParser {
   private:
    std::vector<patternClauseInfo> allClauseInfo;
    std::string currentSynonymName;
    std::vector<std::shared_ptr<QueryToken>> exprTokens;
    std::vector<std::shared_ptr<QueryToken>>::const_iterator parsePatternCond(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    std::vector<std::shared_ptr<QueryToken>>::const_iterator parsePattern(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    // Parse the expression tokens and stores in exprTokens
    std::vector<std::shared_ptr<QueryToken>>::const_iterator storeExprTokens(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    std::shared_ptr<ExprNode> buildPatternAST(int startIndex, int endIndex);
    int findRootIndex(int startIndex, int endIndex);
    int findCloseParenthesis(int startIndex, int endIndex);
    bool isExprOperator(std::string str);
    bool isTermOperator(std::string str);

   public:
    PatternClauseParser();
    explicit PatternClauseParser(std::shared_ptr<SyntaxValidator> syntaxValidator);
    std::vector<std::shared_ptr<QueryToken>>::const_iterator parseClause(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) override;

    std::vector<struct patternClauseInfo> getClauseInfo();
};
