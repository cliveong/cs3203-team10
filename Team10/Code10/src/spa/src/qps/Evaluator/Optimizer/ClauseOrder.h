#pragma once

#include <map>
#include <string>
#include <vector>

#include "pkb/ReadFacade.h"
#include "qps/Clause/Clause.h"
#include "qps/Synonym.h"

class ClauseOrder {
   private:
    std::map<SynonymType, int> entityCountMap;
    int getEntityCount(std::shared_ptr<Synonym> synonym, std::shared_ptr<ReadFacade> readFacade);
    int getScore(std::shared_ptr<Clause> clause, std::shared_ptr<ReadFacade> readFacade);

   public:
    std::vector<std::shared_ptr<Clause>> arrangeClauses(const std::vector<std::shared_ptr<Clause>>& clauseList, const std::shared_ptr<ReadFacade>& readFacade);
};
