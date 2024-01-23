#pragma once

#include <functional>

#include "qps/Evaluator/ClauseEvaluator.h"

class StmtStmtClauseEvaluator : public ClauseEvaluator {
   private:
    bool evaluateWildcardWildcard();
    bool evaluateWildcardInteger();
    bool evaluateWildcardSynonym();
    bool evaluateIntegerWildcard();
    bool evaluateIntegerInteger();
    bool evaluateIntegerSynonym();
    bool evaluateSynonymWildcard();
    bool evaluateSynonymInteger();
    bool evaluateSynonymSynonym();

   protected:
    std::function<std::vector<int>(int)> getStmtsFirstArg;
    std::function<std::vector<int>(int)> getStmtsSecondArg;
    std::function<bool(void)> getAllStmts;

   public:
    StmtStmtClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
    bool evaluateClause() override;
};
