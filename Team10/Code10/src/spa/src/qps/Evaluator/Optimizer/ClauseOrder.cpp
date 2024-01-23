#include "ClauseOrder.h"

#include <algorithm>
#include <unordered_set>
#include <utility>

#include "qps/Utils.h"

int ClauseOrder::getEntityCount(std::shared_ptr<Synonym> synonym, std::shared_ptr<ReadFacade> readFacade) {
    if (entityCountMap.find(synonym->getSynonymType()) != entityCountMap.end()) {
        return entityCountMap[synonym->getSynonymType()];
    } else {
        int entitySize = Utils::getEntitiesList(synonym->getSynonymType(), readFacade).size();
        entityCountMap.insert({synonym->getSynonymType(), entitySize});
        return entitySize;
    }
}

bool comparator(std::pair<int, std::shared_ptr<Clause>> pair1, std::pair<int, std::shared_ptr<Clause>> pair2) {
    return pair1.first < pair2.first;
}

int ClauseOrder::getScore(std::shared_ptr<Clause> clause, std::shared_ptr<ReadFacade> readFacade) {
    int score = 1;
    if (clause->getFirstArg()->isSynonym()) {
        score *= getEntityCount(clause->getFirstArg()->getSynonym(), readFacade);
    }
    if (clause->getSecondArg()->isSynonym()) {
        score *= getEntityCount(clause->getSecondArg()->getSynonym(), readFacade);
    }
    return score;
}

std::vector<std::shared_ptr<Clause>> ClauseOrder::arrangeClauses(const std::vector<std::shared_ptr<Clause>>& clauseList, const std::shared_ptr<ReadFacade>& readFacade) {
    std::vector<std::pair<int, std::shared_ptr<Clause>>> scoredVector;
    scoredVector.reserve(clauseList.size());
    for (auto& clause : clauseList) {
        int score = getScore(clause, readFacade);
        scoredVector.emplace_back(score, clause);
    }
    // Sort from largest to smallest
    std::sort(scoredVector.begin(), scoredVector.end(), comparator);

    std::unordered_set<std::string> evaluatedSynonyms;
    // Select first clause based on smallest score
    std::shared_ptr<Clause> firstClause = scoredVector.front().second;
    std::pair<std::string, std::string> clauseSynonyms = Utils::getSynonyms(firstClause);
    if (!clauseSynonyms.first.empty()) {
        evaluatedSynonyms.insert(clauseSynonyms.first);
    }
    if (!clauseSynonyms.second.empty()) {
        evaluatedSynonyms.insert(clauseSynonyms.second);
    }

    auto clauseIter = scoredVector.begin() + 1;
    while (clauseIter != scoredVector.end()) {
        int counter = 1;
        while (true) {
            std::shared_ptr<Clause> currClause = (*clauseIter).second;
            std::pair<std::string, std::string> currClauseSynonyms = Utils::getSynonyms(currClause);
            if (!currClauseSynonyms.first.empty() && evaluatedSynonyms.find(currClauseSynonyms.first) != evaluatedSynonyms.end()) {
                if (!currClauseSynonyms.second.empty()) {
                    evaluatedSynonyms.insert(currClauseSynonyms.second);
                }
                clauseIter++;
                break;
            }
            if (!currClauseSynonyms.second.empty() && evaluatedSynonyms.find(currClauseSynonyms.second) != evaluatedSynonyms.end()) {
                if (!currClauseSynonyms.first.empty()) {
                    evaluatedSynonyms.insert(currClauseSynonyms.first);
                }
                clauseIter++;
                break;
            }
            // No Common Synonyms, swap
            std::iter_swap(clauseIter, clauseIter + counter);
            counter++;
        }
    }

    std::vector<std::shared_ptr<Clause>> arrangedList;
    arrangedList.reserve(scoredVector.size());
    for (auto& clause : scoredVector) {
        arrangedList.push_back(clause.second);
    }

    return arrangedList;
}
