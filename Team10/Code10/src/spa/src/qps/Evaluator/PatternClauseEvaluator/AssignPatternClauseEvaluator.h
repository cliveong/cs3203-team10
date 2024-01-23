#pragma once

#include "PatternClauseEvaluator.h"

class AssignPatternClauseEvaluator : public PatternClauseEvaluator {
   public:
    AssignPatternClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
};
