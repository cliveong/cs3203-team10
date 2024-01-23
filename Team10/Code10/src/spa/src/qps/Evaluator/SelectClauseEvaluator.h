#pragma once

#include <memory>

#include "pkb/ReadFacade.h"
#include "qps/Clause/SelectClause.h"
#include "qps/ResultsTable.h"

class SelectClauseEvaluator {
   public:
    bool evaluate(const std::shared_ptr<SelectClause>& selectClause, const std::shared_ptr<ResultsTable>& resultsTable, const std::shared_ptr<ReadFacade>& readFacade);
};