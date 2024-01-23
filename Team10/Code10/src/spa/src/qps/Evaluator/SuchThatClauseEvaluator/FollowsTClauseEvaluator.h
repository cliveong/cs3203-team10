#pragma once

#include "StmtStmtClauseEvaluator.h"

class FollowsTClauseEvaluator : public StmtStmtClauseEvaluator {
   public:
    FollowsTClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
};
