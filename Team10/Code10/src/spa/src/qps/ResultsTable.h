#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Elem.h"
#include "Synonym.h"
#include "pkb/ReadFacade.h"

class ResultsTable {
   private:
    std::map<std::string, int> synonymMap;
    std::vector<std::vector<std::string>> resultsTable;
    void insertSynonym(const std::string& synonym);
    void addSingleResultEmpty(const std::string& synonym, std::vector<std::string> resultList);
    void crossJoinSingleResult(const std::string& synonym, std::vector<std::string> resultList);
    void naturalJoinSingleResult(const std::string& synonym, std::vector<std::string> resultList);
    void addPairResultEmpty(std::vector<std::string> synonymList, std::vector<std::vector<std::string>> resultList);
    void crossJoinPairResult(std::vector<std::vector<std::string>> resultList);
    void joinOneCommonPairResult(std::vector<std::vector<std::string>> resultList, int currIndex, int newIndex);
    void joinTwoCommonPairResult(std::vector<std::vector<std::string>> resultList, int index1, int index2);

   public:
    ResultsTable();
    std::vector<std::string> getResult(const std::string& synonym);
    std::vector<std::string> getResult(const std::vector<std::shared_ptr<Elem>>& elementList, const std::shared_ptr<ReadFacade>& readFacade);
    std::pair<std::map<int, std::string>, std::vector<std::vector<std::string>>> getSynonymResults(const std::vector<std::shared_ptr<Synonym>>& synonymList);
    bool addSingleResult(const std::string& synonym, const std::vector<std::string>& resultList);
    bool addPairResult(std::vector<std::string> synonymList, const std::vector<std::vector<std::string>>& resultPairs);
    void combineResults(const std::pair<std::map<int, std::string>, std::vector<std::vector<std::string>>>& partialResults);
    bool hasResult(const std::string& synonym);
};