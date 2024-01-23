#include "ClauseOptimizer.h"

#include "qps/Clause/PatternClause/PatternClause.h"
#include "qps/Utils.h"

std::pair<bool, std::shared_ptr<ClauseGroup>> ClauseOptimizer::findGroup(const std::shared_ptr<Clause>& clause) {
    std::pair<std::string, std::string> synonyms = Utils::getSynonyms(clause);
    std::string synonym1 = synonyms.first;
    std::string synonym2 = synonyms.second;

    if (synonym1.empty() && synonym2.empty()) {  // No Synonym, add to first group
        return std::make_pair(true, clauseGroups.front());
    }
    std::vector<std::shared_ptr<ClauseGroup>> matchingGroups;
    for (auto& clauseGroup : clauseGroups) {
        if ((!synonym1.empty() && clauseGroup->synonymInGroup(synonym1)) || (!synonym2.empty() &&
                                                                             clauseGroup->synonymInGroup(synonym2))) {
            // Found at least one synonym in group, add to vector of matching groups
            matchingGroups.push_back(clauseGroup);
        }
    }
    if (matchingGroups.empty()) {  // Cannot find existing group, create new group
        return std::make_pair(false, clauseGroups.front());
    }
    if (matchingGroups.size() == 1) {  // Found only one matching group, can add to that group
        return std::make_pair(true, matchingGroups.front());
    }
    // Found multiple matching groups, combine groups --> Only 2 groups will be combined at one time
    std::shared_ptr<ClauseGroup> group = combineGroups(matchingGroups);
    return std::make_pair(true, group);
}

void ClauseOptimizer::orderClauses(std::shared_ptr<ReadFacade> readFacade) {
    for (auto group = ++(clauseGroups.begin()); group != clauseGroups.end(); group++) {
        std::vector<std::shared_ptr<Clause>> newClauseList = clauseOrder.arrangeClauses((*group)->getClauses(), readFacade);
        (*group)->setClauseList(newClauseList);
    }
}

std::shared_ptr<ClauseGroup> ClauseOptimizer::combineGroups(std::vector<std::shared_ptr<ClauseGroup>> matchingGroups) {
    std::shared_ptr<ClauseGroup> finalGroup = matchingGroups.front();
    std::shared_ptr<ClauseGroup> otherGroup = matchingGroups.back();
    std::vector<std::shared_ptr<Clause>> clauses = otherGroup->getClauses();
    for (auto& clause : clauses) {
        finalGroup->addClause(clause);
    }

    std::vector<std::shared_ptr<ClauseGroup>> newClauseGroups;
    for (auto& group : clauseGroups) {
        if (group != otherGroup) {
            newClauseGroups.push_back(group);
        }
    }
    clauseGroups = newClauseGroups;
    return finalGroup;
}

std::vector<std::shared_ptr<ClauseGroup>>
ClauseOptimizer::optimizeClauses(std::vector<std::shared_ptr<Clause>> clauses, std::shared_ptr<ReadFacade> readFacade) {
    std::shared_ptr<ClauseGroup> noSynonymGroup = std::make_shared<ClauseGroup>(readFacade);
    clauseGroups.push_back(noSynonymGroup);
    // Group clauses
    for (auto& clause : clauses) {
        std::pair<bool, std::shared_ptr<ClauseGroup>> group = findGroup(clause);
        if (group.first) {  // Group found
            group.second->addClause(clause);
        } else {  // Group not found: create new group
            std::shared_ptr<ClauseGroup> newGroup = std::make_shared<ClauseGroup>(readFacade);
            newGroup->addClause(clause);
            clauseGroups.push_back(newGroup);
        }
    }
    // Order Clauses
    orderClauses(readFacade);
    // Return result
    return clauseGroups;
}
