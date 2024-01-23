#pragma once

#include <functional>

#include "commons/ASTNode.h"
#include "qps/Clause/PatternClause/PatternClause.h"
#include "qps/Evaluator/ClauseEvaluator.h"
#include "qps/ExprArgument.h"
#include "sp/Parsers/ExprParser.h"

class PatternClauseEvaluator : public ClauseEvaluator {
   private:
    bool evaluateWildcardWildcard(const std::shared_ptr<PatternClause>& patternClause);
    bool evaluateWildcardPattern(const std::shared_ptr<PatternClause>& patternClause);
    bool evaluateStringWildcard(const std::shared_ptr<PatternClause>& patternClause);
    bool evaluateStringPattern(const std::shared_ptr<PatternClause>& patternClause);
    bool evaluateVariableWildcard(const std::shared_ptr<PatternClause>& patternClause);
    bool evaluateVariablePattern(const std::shared_ptr<PatternClause>& patternClause);

   protected:
    std::function<std::vector<int>(void)> getWildcardStmts;
    std::function<std::vector<int>(std::string)> getPatternLHS;
    std::function<std::vector<int>(ASTNode, bool)> getPatternRHS;
    std::function<std::vector<int>(std::string, ASTNode, bool)> getPatternStmts;

   public:
    PatternClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
    bool evaluateClause() override;
};
