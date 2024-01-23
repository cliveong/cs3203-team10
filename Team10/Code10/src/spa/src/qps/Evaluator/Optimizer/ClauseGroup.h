#pragma once

#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "pkb/ReadFacade.h"
#include "qps/Clause/Clause.h"
#include "qps/ResultsTable.h"
#include "qps/Synonym.h"

class ClauseGroup {
   private:
    std::shared_ptr<ReadFacade> readFacade;
    std::unordered_set<std::string> synonymGroup;
    std::vector<std::shared_ptr<Clause>> clauseList;
    std::shared_ptr<ResultsTable> resultsTable = std::make_shared<ResultsTable>();

   public:
    ClauseGroup(std::shared_ptr<ReadFacade> readFacade);
    bool operator==(const ClauseGroup& clauseGroup);
    bool synonymInGroup(const std::string& synonymName);
    void addClause(const std::shared_ptr<Clause>& clause);
    std::vector<std::shared_ptr<Clause>> getClauses();
    std::shared_ptr<ResultsTable> getResultsTable();
    void setClauseList(std::vector<std::shared_ptr<Clause>> newClauseList);
    bool evaluateGroup();
    std::pair<std::map<int, std::string>, std::vector<std::vector<std::string>>> getResults(const std::vector<std::shared_ptr<Synonym>>& synonyms);
};
