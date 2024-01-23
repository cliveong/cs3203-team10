#include "WithClauseEvaluator.h"

#include "qps/Utils.h"
#include "qps/WithArgument.h"

WithClauseEvaluator::WithClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                         std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : ClauseEvaluator(resultsTable, readFacade, clause) {}

bool WithClauseEvaluator::evaluateNonSynonyms(std::shared_ptr<Argument> firstArg, std::shared_ptr<Argument> secondArg) {
    return clause->isNot() ^ (firstArg->getValue() == secondArg->getValue());
}

bool WithClauseEvaluator::evaluateStringSynonym(std::shared_ptr<Argument> stringArg, std::shared_ptr<Argument> synonymArg) {
    std::string stringValue = stringArg->getValue();
    std::shared_ptr<Synonym> synonym = synonymArg->getSynonym();
    if (synonym->getSynonymType() == PROCEDURE || synonym->getSynonymType() == VARIABLE) {
        std::vector<std::string> entityList = Utils::getEntitiesList(synonym, readFacade, resultsTable);
        std::vector<std::string> results;
        for (auto& i : entityList) {
            if (clause->isNot() ^ i == stringValue) {
                results.push_back(i);
            }
        }
        return resultsTable->addSingleResult(synonym->getSynonymName(), results);
    } else if (synonym->getSynonymType() == CALL || synonym->getSynonymType() == READ || synonym->getSynonymType() == PRINT) {
        std::vector<std::pair<std::string, std::string>> entityPairs = Utils::getEntityPairs(synonym, readFacade, resultsTable);
        std::vector<std::string> results;
        for (auto& pair : entityPairs) {
            if (clause->isNot() ^ pair.second == stringValue) {
                results.push_back(pair.first);
            }
        }
        return resultsTable->addSingleResult(synonym->getSynonymName(), results);
    }
    return false;
}

bool WithClauseEvaluator::evaluateIntegerSynonym(std::shared_ptr<Argument> intArg, std::shared_ptr<Argument> synonymArg) {
    std::string intValue = intArg->getValue();
    std::shared_ptr<Synonym> synonym = synonymArg->getSynonym();
    std::vector<std::string> synonymIntList = Utils::getEntitiesList(synonym, readFacade, resultsTable);
    std::vector<std::string> results;
    for (auto& i : synonymIntList) {
        if (clause->isNot() ^ i == intValue) {
            results.push_back(i);
        }
    }
    return resultsTable->addSingleResult(synonym->getSynonymName(), results);
}

bool WithClauseEvaluator::evaluateVectorComparisons(std::shared_ptr<Argument> firstArg, std::shared_ptr<Argument> secondArg) {
    std::vector<std::string> firstEntityList = Utils::getEntitiesList(firstArg->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> secondEntityList = Utils::getEntitiesList(secondArg->getSynonym(), readFacade, resultsTable);
    std::vector<std::vector<std::string>> results = Utils::getCommonElements(firstEntityList, secondEntityList, clause->isNot());
    return resultsTable->addPairResult({firstArg->getSynonym()->getSynonymName(), secondArg->getSynonym()->getSynonymName()}, results);
}

bool WithClauseEvaluator::evaluatePairComparisons(std::shared_ptr<Argument> firstArg, std::shared_ptr<Argument> secondArg) {
    std::vector<std::pair<std::string, std::string>> firstEntityList = Utils::getEntityPairs(firstArg->getSynonym(), readFacade, resultsTable);
    std::vector<std::pair<std::string, std::string>> secondEntityList = Utils::getEntityPairs(secondArg->getSynonym(), readFacade, resultsTable);
    std::vector<std::vector<std::string>> results = Utils::getCommonElements(firstEntityList, secondEntityList, clause->isNot());
    return resultsTable->addPairResult({firstArg->getSynonym()->getSynonymName(), secondArg->getSynonym()->getSynonymName()}, results);
}

bool WithClauseEvaluator::evaluateSynonymSynonym(std::shared_ptr<Argument> firstArg, std::shared_ptr<Argument> secondArg) {
    if (*firstArg == *secondArg) {
        if (clause->isNot()) {
            return false;
        }
        std::shared_ptr<Synonym> synonym = firstArg->getSynonym();
        std::vector<std::string> entityList = Utils::getEntitiesList(synonym, readFacade, resultsTable);
        return resultsTable->addSingleResult(synonym->getSynonymName(), entityList);
    }

    std::shared_ptr<WithArgument> arg1 = std::dynamic_pointer_cast<WithArgument>(firstArg);
    if (firstArg->getSynonym()->getSynonymType() == secondArg->getSynonym()->getSynonymType() || arg1->getAttribute()->getAttrType() == INTEGER_ATTRTYPE) {
        return evaluateVectorComparisons(firstArg, secondArg);
    }

    if (arg1->getAttribute()->getAttrType() == NAME_ATTRTYPE) {
        SynonymType firstType = firstArg->getSynonym()->getSynonymType();
        SynonymType secondType = secondArg->getSynonym()->getSynonymType();
        if ((firstType == PROCEDURE || firstType == VARIABLE) && (secondType == PROCEDURE || secondType == VARIABLE)) {
            return evaluateVectorComparisons(firstArg, secondArg);
        } else {
            return evaluatePairComparisons(firstArg, secondArg);
        }
    }

    return true;
}

bool WithClauseEvaluator::evaluateClause() {
    std::shared_ptr<Argument> firstArg = clause->getFirstArg();
    std::shared_ptr<Argument> secondArg = clause->getSecondArg();

    if (firstArg->getType() == STRING_ARG) {
        if (secondArg->getType() == STRING_ARG) {
            return evaluateNonSynonyms(firstArg, secondArg);
        } else if (secondArg->getType() == SYNONYM) {
            return evaluateStringSynonym(firstArg, secondArg);
        }
    } else if (firstArg->getType() == INT_ARG) {
        if (secondArg->getType() == INT_ARG) {
            return evaluateNonSynonyms(firstArg, secondArg);
        } else if (secondArg->getType() == SYNONYM) {
            return evaluateIntegerSynonym(firstArg, secondArg);
        }
    } else if (firstArg->getType() == SYNONYM) {
        if (secondArg->getType() == STRING_ARG) {
            return evaluateStringSynonym(secondArg, firstArg);
        } else if (secondArg->getType() == INT_ARG) {
            return evaluateIntegerSynonym(secondArg, firstArg);
        } else if (secondArg->getType() == SYNONYM) {
            return evaluateSynonymSynonym(firstArg, secondArg);
        }
    }
    return true;
}
