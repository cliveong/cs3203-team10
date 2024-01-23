#include "ResultsTable.h"
#include "AttrRef.h"
#include "qps/Utils.h"

#include <algorithm>
#include <sstream>
#include <unordered_set>
#include <utility>

ResultsTable::ResultsTable() = default;

std::vector<std::string> ResultsTable::getResult(const std::vector<std::shared_ptr<Elem>>& elementList, const std::shared_ptr<ReadFacade>& readFacade) {
    std::vector<std::string> result;
    if (resultsTable.empty()) {
        return result;
    }
    std::vector<int> indexList;
    indexList.reserve(elementList.size());
    for (auto& element : elementList) {
        indexList.push_back(synonymMap[Utils::getSynonym(element)->getSynonymName()]);
    }
    std::unordered_set<std::string> resultSet;
    resultSet.reserve(resultsTable.size());
    for (auto row = resultsTable.begin(); row != resultsTable.end(); row++) {
        std::ostringstream rowStream;
        for (int i = 0; i < elementList.size(); i++) {
            int index = indexList[i];
            const std::shared_ptr<Elem>& element = elementList[i];
            const std::string& synResult = (*row)[index];
            if (element->isAttrRef()) {
                rowStream << Utils::getAttrRef(element, synResult, readFacade) << " ";
            } else {
                rowStream << synResult << " ";
            }
        }
        std::string rowString = rowStream.str();
        rowString.pop_back();
        resultSet.insert(std::move(rowString));
    }
    return {resultSet.begin(), resultSet.end()};
}

std::vector<std::string> ResultsTable::getResult(const std::string& synonym) {
    std::vector<std::string> result;
    if (synonymMap.find(synonym) == synonymMap.end()) {
        return result;
    }

    int index = synonymMap[synonym];
    std::unordered_set<std::string> resultSet;
    resultSet.reserve(resultsTable.size());
    for (auto i = resultsTable.begin(); i != resultsTable.end(); i++) {
        resultSet.insert((*i)[index]);
    }
    return {resultSet.begin(), resultSet.end()};
}

bool ResultsTable::hasResult(const std::string& synonym) {
    return synonymMap.find(synonym) != synonymMap.end();
}

struct MyHash {
    std::size_t operator()(const std::vector<std::string>& stringVector) const noexcept {
        std::size_t h1 = 0;
        for (auto& s : stringVector) {
            h1 += std::hash<std::string>{}(s);
        }
        return h1;
    }
};

std::pair<std::map<int, std::string>, std::vector<std::vector<std::string>>>
ResultsTable::getSynonymResults(const std::vector<std::shared_ptr<Synonym>>& synonymList) {
    std::map<int, std::string> synonymIndexMap;
    std::vector<std::vector<std::string>> synonymResults;
    synonymResults.reserve(resultsTable.size());

    std::vector<int> currIndexList;
    int indexCount = 0;
    for (auto& synonym : synonymList) {
        if (synonymMap.find(synonym->getSynonymName()) != synonymMap.end()) {
            currIndexList.push_back(synonymMap[synonym->getSynonymName()]);
            synonymIndexMap.insert({indexCount, synonym->getSynonymName()});
            indexCount++;
        }
    }
    if (synonymIndexMap.empty()) {
        return std::make_pair(synonymIndexMap, synonymResults);
    }
    std::unordered_set<std::vector<std::string>, MyHash> resultSet;
    resultSet.reserve(resultsTable.size());
    for (auto& row : resultsTable) {
        std::vector<std::string> rowResult;
        rowResult.reserve(currIndexList.size());
        for (auto& index : currIndexList) {
            rowResult.push_back(row[index]);
        }
        resultSet.insert(rowResult);
    }
    synonymResults.assign(resultSet.begin(), resultSet.end());
    return std::make_pair(synonymIndexMap, synonymResults);
}

void ResultsTable::combineResults(const std::pair<std::map<int, std::string>, std::vector<std::vector<std::string>>>& partialResults) {
    std::map<int, std::string> newSynonymMap = partialResults.first;
    std::vector<std::vector<std::string>> newResults = partialResults.second;
    if (synonymMap.empty()) {
        for (int i = 0; i < newSynonymMap.size(); i++) {
            synonymMap.insert({newSynonymMap[i], i});
        }
        resultsTable = newResults;
        return;
    }
    // Join synonym maps
    int currIndex = synonymMap.size();
    for (int i = 0; i < newSynonymMap.size(); i++) {
        synonymMap.insert({newSynonymMap[i], currIndex + i});
    }
    // Cross join tables
    std::vector<std::vector<std::string>> newResultsTable;
    newResultsTable.reserve(resultsTable.size() * newResults.size());
    for (auto& currRow : resultsTable) {
        for (auto& newRow : newResults) {
            std::vector<std::string> combinedRow;
            combinedRow.reserve(currRow.size() + newRow.size());
            combinedRow.insert(combinedRow.end(), currRow.begin(), currRow.end());
            combinedRow.insert(combinedRow.end(), newRow.begin(), newRow.end());
            newResultsTable.emplace_back(std::move(combinedRow));
        }
    }
    resultsTable.swap(newResultsTable);
}

void ResultsTable::insertSynonym(const std::string& synonym) {
    int newIndex = synonymMap.size();
    synonymMap.insert({synonym, newIndex});
}

void ResultsTable::addSingleResultEmpty(const std::string& synonym, std::vector<std::string> resultList) {
    for (auto i = resultList.begin(); i != resultList.end(); i++) {
        resultsTable.push_back({*i});
    }
    insertSynonym(synonym);
}

void ResultsTable::crossJoinSingleResult(const std::string& synonym, std::vector<std::string> resultList) {
    std::vector<std::vector<std::string>> newResultsTable;
    newResultsTable.reserve(resultsTable.size() * resultList.size());
    for (auto currRes = resultsTable.begin(); currRes != resultsTable.end(); currRes++) {
        for (auto newRes = resultList.begin(); newRes != resultList.end(); newRes++) {
            (*currRes).push_back(*newRes);
            newResultsTable.push_back(*currRes);
            (*currRes).pop_back();
        }
    }
    resultsTable.swap(newResultsTable);
    insertSynonym(synonym);
}

void ResultsTable::naturalJoinSingleResult(const std::string& synonym, std::vector<std::string> resultList) {
    std::vector<std::vector<std::string>> newResultsTable;
    newResultsTable.reserve(resultsTable.size());
    int index = synonymMap[synonym];
    for (auto currRes = resultsTable.begin(); currRes != resultsTable.end(); currRes++) {
        if (std::find(resultList.begin(), resultList.end(), (*currRes)[index]) != resultList.end()) {
            newResultsTable.push_back(*currRes);
        }
    }
    resultsTable.swap(newResultsTable);
}

void ResultsTable::addPairResultEmpty(std::vector<std::string> synonymList, std::vector<std::vector<std::string>> resultList) {
    resultsTable = std::move(resultList);
    insertSynonym(synonymList[0]);
    insertSynonym(synonymList[1]);
}

void ResultsTable::crossJoinPairResult(std::vector<std::vector<std::string>> resultList) {
    std::vector<std::vector<std::string>> newResultsTable;
    newResultsTable.reserve(resultsTable.size() * resultList.size());
    for (auto currRes = resultsTable.begin(); currRes != resultsTable.end(); currRes++) {
        for (auto newRes = resultList.begin(); newRes != resultList.end(); newRes++) {
            (*currRes).push_back((*newRes)[0]);
            (*currRes).push_back((*newRes)[1]);
            newResultsTable.push_back(*currRes);
            (*currRes).pop_back();
            (*currRes).pop_back();
        }
    }
    resultsTable.swap(newResultsTable);
}

void ResultsTable::joinOneCommonPairResult(std::vector<std::vector<std::string>> resultList, int currIndex, int newIndex) {
    int otherIndex = newIndex == 0 ? 1 : 0;
    std::vector<std::vector<std::string>> newResultsTable;
    newResultsTable.reserve(resultsTable.size() * resultList.size());
    for (auto currRes = resultsTable.begin(); currRes != resultsTable.end(); currRes++) {
        for (auto newRes = resultList.begin(); newRes != resultList.end(); newRes++) {
            if ((*currRes)[currIndex] == (*newRes)[newIndex]) {
                (*currRes).push_back((*newRes)[otherIndex]);
                newResultsTable.push_back(*currRes);
                (*currRes).pop_back();
            }
        }
    }
    resultsTable.swap(newResultsTable);
}

void ResultsTable::joinTwoCommonPairResult(std::vector<std::vector<std::string>> resultList, int index1, int index2) {
    std::vector<std::vector<std::string>> newResultsTable;
    newResultsTable.reserve(resultsTable.size());
    for (auto currRes = resultsTable.begin(); currRes != resultsTable.end(); currRes++) {
        for (auto newRes = resultList.begin(); newRes != resultList.end(); newRes++) {
            if ((*currRes)[index1] == (*newRes)[0] && (*currRes)[index2] == (*newRes)[1]) {
                newResultsTable.push_back(*currRes);
                break;
            }
        }
    }
    resultsTable.swap(newResultsTable);
}

bool ResultsTable::addSingleResult(const std::string& synonym, const std::vector<std::string>& resultList) {
    if (resultList.empty()) {
        return false;
    }
    // If empty results table, add results into table
    if (resultsTable.empty()) {
        addSingleResultEmpty(synonym, resultList);
    } else if (synonymMap.find(synonym) == synonymMap.end()) {  // If synonym not in map, perform cross join
        crossJoinSingleResult(synonym, resultList);
    } else {  // If synonym in map, perform natural join
        naturalJoinSingleResult(synonym, resultList);
    }

    if (resultsTable.empty()) {
        return false;
    }
    return true;
}

bool ResultsTable::addPairResult(std::vector<std::string> synonymList, const std::vector<std::vector<std::string>>& resultList) {
    if (resultList.empty()) {
        return false;
    }
    // If empty results, add both results to table, add both synonym
    std::string firstSyn = synonymList[0];
    std::string secondSyn = synonymList[1];
    auto firstIter = synonymMap.find(firstSyn);
    auto secondIter = synonymMap.find(secondSyn);
    if (resultsTable.empty()) {
        addPairResultEmpty(synonymList, resultList);
    } else if (firstIter == synonymMap.end() && secondIter == synonymMap.end()) {  // If no common synonym, cross join
        crossJoinPairResult(resultList);
        insertSynonym(firstSyn);
        insertSynonym(secondSyn);
    } else if (firstIter != synonymMap.end() && secondIter != synonymMap.end()) {  // If 2 common synonym, natural join based on both
        int index1 = firstIter->second;
        int index2 = secondIter->second;
        joinTwoCommonPairResult(resultList, index1, index2);
    } else if (firstIter != synonymMap.end()) {  // If one common synonym, natural join based on the similar synonym
        int index = firstIter->second;
        joinOneCommonPairResult(resultList, index, 0);
        insertSynonym(secondSyn);
    } else {  // one common synonym but for second one
        int index = secondIter->second;
        joinOneCommonPairResult(resultList, index, 1);
        insertSynonym(firstSyn);
    }

    if (resultsTable.empty()) {
        return false;
    }
    return true;
}
