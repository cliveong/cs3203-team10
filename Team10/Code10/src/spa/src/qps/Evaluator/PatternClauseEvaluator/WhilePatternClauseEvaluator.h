#pragma once

#include "PatternClauseEvaluator.h"

class WhilePatternClauseEvaluator : public PatternClauseEvaluator {
   public:
    WhilePatternClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
};
