#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "pkb/ReadFacade.h"
#include "qps/Clause/Clause.h"
#include "qps/Evaluator/Optimizer/ClauseGroup.h"
#include "qps/Evaluator/Optimizer/ClauseOrder.h"
#include "qps/ResultsTable.h"
#include "qps/Synonym.h"

class ClauseOptimizer {
   private:
    ClauseOrder clauseOrder = ClauseOrder();
    std::vector<std::shared_ptr<ClauseGroup>> clauseGroups;
    std::pair<bool, std::shared_ptr<ClauseGroup>> findGroup(const std::shared_ptr<Clause>& clause);
    void orderClauses(std::shared_ptr<ReadFacade> readFacade);
    std::shared_ptr<ClauseGroup> combineGroups(std::vector<std::shared_ptr<ClauseGroup>> matchingGroups);

   public:
    std::vector<std::shared_ptr<ClauseGroup>> optimizeClauses(std::vector<std::shared_ptr<Clause>> clauses, std::shared_ptr<ReadFacade> readFacade);
};
