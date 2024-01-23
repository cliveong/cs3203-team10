#pragma once

#include "PatternClauseEvaluator.h"

class IfPatternClauseEvaluator : public PatternClauseEvaluator {
   public:
    IfPatternClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
};
