#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Optimizer/ClauseGroup.h"
#include "pkb/ReadFacade.h"
#include "qps/Query.h"
#include "qps/ResultsTable.h"

class QueryEvaluator {
   private:
    std::shared_ptr<Query> query;
    std::shared_ptr<ReadFacade> readFacade;
    std::shared_ptr<ResultsTable> getEvaluatedElements(const std::vector<std::shared_ptr<ClauseGroup>>& clauseGroups,
                                                       const std::vector<std::shared_ptr<Elem>>& elemList);

   public:
    QueryEvaluator(std::shared_ptr<Query> query, std::shared_ptr<ReadFacade> readFacade);
    std::vector<std::string> evaluateQuery();
};