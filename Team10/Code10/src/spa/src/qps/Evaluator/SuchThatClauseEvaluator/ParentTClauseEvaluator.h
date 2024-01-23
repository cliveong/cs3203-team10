#pragma once

#include "StmtStmtClauseEvaluator.h"

class ParentTClauseEvaluator : public StmtStmtClauseEvaluator {
   public:
    ParentTClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
};
