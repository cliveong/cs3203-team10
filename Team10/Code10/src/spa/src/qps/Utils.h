#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "Argument.h"
#include "ResultsTable.h"
#include "Synonym.h"
#include "pkb/ReadFacade.h"
#include "qps/Clause/Clause.h"

class Utils {
   private:
    static std::vector<int> convertToIntVector(const std::vector<std::string>& stringVector);
    static std::vector<std::string> convertToStringVector(const std::vector<int>& intVector);
    static std::string getName(SynonymType synonymType, int stmtNum, const std::shared_ptr<ReadFacade>& readFacade);

   public:
    static std::vector<int> getStmtList(const std::shared_ptr<Synonym>& synonym, std::shared_ptr<ReadFacade> readFacade, const std::shared_ptr<ResultsTable>& resultsTable);
    static std::vector<int> getStmtList(SynonymType stmtType, const std::shared_ptr<ReadFacade>& readFacade);
    static std::vector<std::string> getEntitiesList(const std::shared_ptr<Synonym>& synonym, std::shared_ptr<ReadFacade> readFacade, const std::shared_ptr<ResultsTable>& resultsTable);
    static std::vector<std::string> getEntitiesList(SynonymType synonymType, const std::shared_ptr<ReadFacade>& readFacade);
    static bool isStatement(const std::shared_ptr<Argument>& argument);
    static std::vector<std::pair<std::string, std::string>> getEntityPairs(const std::shared_ptr<Synonym>& synonym, const std::shared_ptr<ReadFacade>& readFacade, const std::shared_ptr<ResultsTable>& resultsTable);
    static std::vector<std::vector<std::string>> getCommonElements(std::vector<std::string> v1, std::vector<std::string> v2, bool isNot);
    static std::vector<std::vector<std::string>> getCommonElements(std::vector<std::pair<std::string, std::string>> p1, std::vector<std::pair<std::string, std::string>> p2, bool isNot);
    static std::pair<std::string, std::string> getSynonyms(const std::shared_ptr<Clause>& clause);
    static std::shared_ptr<Synonym> getSynonym(const std::shared_ptr<Elem>& element);
    static std::string getAttrRef(const std::shared_ptr<Elem>& element, const std::string& value, const std::shared_ptr<ReadFacade>& readFacade);
};