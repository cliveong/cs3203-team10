#pragma once

#include "StmtEntClauseEvaluator.h"

class UsesClauseEvaluator : public StmtEntClauseEvaluator {
   public:
    UsesClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
};
