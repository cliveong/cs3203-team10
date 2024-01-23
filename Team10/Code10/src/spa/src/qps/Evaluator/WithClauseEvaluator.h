#pragma once

#include "qps/Evaluator/ClauseEvaluator.h"

class WithClauseEvaluator : public ClauseEvaluator {
   private:
    bool evaluateNonSynonyms(std::shared_ptr<Argument> firstArg, std::shared_ptr<Argument> secondArg);
    bool evaluateStringSynonym(std::shared_ptr<Argument> firstArg, std::shared_ptr<Argument> secondArg);
    bool evaluateIntegerSynonym(std::shared_ptr<Argument> firstArg, std::shared_ptr<Argument> secondArg);
    bool evaluateVectorComparisons(std::shared_ptr<Argument> firstArg, std::shared_ptr<Argument> secondArg);
    bool evaluatePairComparisons(std::shared_ptr<Argument> firstArg, std::shared_ptr<Argument> secondArg);
    bool evaluateSynonymSynonym(std::shared_ptr<Argument> firstArg, std::shared_ptr<Argument> secondArg);

   public:
    WithClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
    bool evaluateClause() override;
};
