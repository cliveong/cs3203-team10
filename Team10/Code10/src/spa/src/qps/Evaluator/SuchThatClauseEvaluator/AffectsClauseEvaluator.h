#pragma once

#include "StmtStmtClauseEvaluator.h"

class AffectsClauseEvaluator : public StmtStmtClauseEvaluator {
   public:
    AffectsClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
};
