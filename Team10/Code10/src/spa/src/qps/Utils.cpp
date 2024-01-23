#include "Utils.h"

#include <map>
#include <utility>

#include "AttrRef.h"
#include "qps/Clause/PatternClause/PatternClause.h"

std::vector<int> Utils::convertToIntVector(const std::vector<std::string>& stringVector) {
    std::vector<int> intVector;
    intVector.reserve(stringVector.size());
    for (auto& i : stringVector) {
        intVector.push_back(std::stoi(i));
    }
    return intVector;
}

std::vector<std::string> Utils::convertToStringVector(const std::vector<int>& intVector) {
    std::vector<std::string> stringVector;
    stringVector.reserve(intVector.size());
    for (auto& i : intVector) {
        stringVector.push_back(std::to_string(i));
    }
    return stringVector;
}

std::vector<int> Utils::getStmtList(const std::shared_ptr<Synonym>& synonym, std::shared_ptr<ReadFacade> readFacade, const std::shared_ptr<ResultsTable>& resultsTable) {
    std::string synonymName = synonym->getSynonymName();
    std::vector<std::string> existingResults = resultsTable->getResult(synonymName);
    if (existingResults.empty()) {
        return getStmtList(synonym->getSynonymType(), std::move(readFacade));
    }
    return convertToIntVector(existingResults);
}

std::vector<int> Utils::getStmtList(SynonymType stmtType, const std::shared_ptr<ReadFacade>& readFacade) {
    if (stmtType == STMT) {
        return readFacade->getStmt();
    }
    if (stmtType == READ) {
        return readFacade->getReadS();
    }
    if (stmtType == PRINT) {
        return readFacade->getPrintS();
    }
    if (stmtType == WHILE) {
        return readFacade->getWhile();
    }
    if (stmtType == IF) {
        return readFacade->getIf();
    }
    if (stmtType == ASSIGN) {
        return readFacade->getAssign();
    }
    if (stmtType == CALL) {
        return readFacade->getAllCallStmtToProcName();
    }
    return {};
}

bool Utils::isStatement(const std::shared_ptr<Argument>& argument) {
    if (!argument->isSynonym()) {
        return false;
    }
    std::shared_ptr<Synonym> synonym = argument->getSynonym();
    SynonymType synonymType = synonym->getSynonymType();
    std::vector<SynonymType> stmts = {STMT, READ, PRINT, WHILE, IF, ASSIGN, CALL};
    return std::find(stmts.begin(), stmts.end(), synonymType) != stmts.end();
}

std::vector<std::string> Utils::getEntitiesList(const std::shared_ptr<Synonym>& synonym, std::shared_ptr<ReadFacade> readFacade, const std::shared_ptr<ResultsTable>& resultsTable) {
    std::string synonymName = synonym->getSynonymName();
    std::vector<std::string> existingResults = resultsTable->getResult(synonymName);
    if (existingResults.empty()) {
        return getEntitiesList(synonym->getSynonymType(), std::move(readFacade));
    }
    return existingResults;
}

std::vector<std::string> Utils::getEntitiesList(SynonymType synonymType, const std::shared_ptr<ReadFacade>& readFacade) {
    switch (synonymType) {
        case STMT:
            return convertToStringVector(readFacade->getStmt());
        case READ:
            return convertToStringVector(readFacade->getReadS());
        case PRINT:
            return convertToStringVector(readFacade->getPrintS());
        case WHILE:
            return convertToStringVector(readFacade->getWhile());
        case IF:
            return convertToStringVector(readFacade->getIf());
        case ASSIGN:
            return convertToStringVector(readFacade->getAssign());
        case CALL:
            return convertToStringVector(readFacade->getAllCallStmtToProcName());
        case CONSTANT:
            return readFacade->getConstant();
        case PROCEDURE:
            return readFacade->getProcedure();
        case VARIABLE:
            return readFacade->getVariable();
        default:
            return {};
    }
}

std::string Utils::getName(SynonymType synonymType, int stmtNum, const std::shared_ptr<ReadFacade>& readFacade) {
    switch (synonymType) {
        case CALL:
            return readFacade->getCallStmtToProcName(stmtNum);
        case PRINT:
            return readFacade->getPrintVarName(stmtNum);
        case READ:
            return readFacade->getReadVarName(stmtNum);
        default:
            return std::to_string(stmtNum);
    }
}

std::vector<std::pair<std::string, std::string>> Utils::getEntityPairs(const std::shared_ptr<Synonym>& synonym, const std::shared_ptr<ReadFacade>& readFacade, const std::shared_ptr<ResultsTable>& resultsTable) {
    std::vector<std::pair<std::string, std::string>> entityPairs;
    std::vector<std::string> pairFirstValues = getEntitiesList(synonym, readFacade, resultsTable);
    if (synonym->getSynonymType() == PROCEDURE || synonym->getSynonymType() == VARIABLE) {
        for (auto& value : pairFirstValues) {
            entityPairs.emplace_back(value, value);
        }
    } else {
        for (auto& value : pairFirstValues) {
            entityPairs.emplace_back(value, getName(synonym->getSynonymType(), std::stoi(value), readFacade));
        }
    }
    return entityPairs;
}

std::vector<std::vector<std::string>> Utils::getCommonElements(std::vector<std::string> v1, std::vector<std::string> v2, bool isNot) {
    std::vector<std::vector<std::string>> results;
    for (auto& string1 : v1) {
        for (auto& string2 : v2) {
            if (isNot ^ string1 == string2) {
                results.push_back({string1, string2});
            }
        }
    }
    return results;
}

std::vector<std::vector<std::string>> Utils::getCommonElements(std::vector<std::pair<std::string, std::string>> p1, std::vector<std::pair<std::string, std::string>> p2, bool isNot) {
    std::vector<std::vector<std::string>> results;
    for (auto& pair1 : p1) {
        for (auto& pair2 : p2) {
            if (isNot ^ pair1.second == pair2.second) {
                results.push_back({pair1.first, pair2.first});
            }
        }
    }
    return results;
}

std::pair<std::string, std::string> Utils::getSynonyms(const std::shared_ptr<Clause>& clause) {
    std::string synonym1;
    std::string synonym2;
    if (clause->getName() == "AssignPattern" || clause->getName() == "IfPattern" || clause->getName() == "WhilePattern") {
        std::shared_ptr<PatternClause> patternClause = std::dynamic_pointer_cast<PatternClause>(clause);
        synonym1 = patternClause->getSynonym()->getSynonymName();
        if (patternClause->getFirstArg()->isSynonym()) {
            synonym2 = patternClause->getFirstArg()->getSynonym()->getSynonymName();
        }
    } else {
        if (clause->getFirstArg()->isSynonym()) {
            synonym1 = clause->getFirstArg()->getSynonym()->getSynonymName();
        }
        if (clause->getSecondArg()->isSynonym()) {
            synonym2 = clause->getSecondArg()->getSynonym()->getSynonymName();
        }
    }
    return std::make_pair(synonym1, synonym2);
}

std::shared_ptr<Synonym> Utils::getSynonym(const std::shared_ptr<Elem>& element) {
    std::shared_ptr<Synonym> synonym;
    if (element->isSynonym()) {
        synonym = std::dynamic_pointer_cast<Synonym>(element);
    } else {
        std::shared_ptr<AttrRef> attrRef = std::dynamic_pointer_cast<AttrRef>(element);
        synonym = attrRef->getSynonym();
    }
    return synonym;
}

std::string Utils::getAttrRef(const std::shared_ptr<Elem>& element, const std::string& value, const std::shared_ptr<ReadFacade>& readFacade) {
    std::shared_ptr<AttrRef> attrRef = std::dynamic_pointer_cast<AttrRef>(element);
    std::shared_ptr<Synonym> synonym = attrRef->getSynonym();
    std::shared_ptr<Attribute> attribute = attrRef->getAttribute();
    SynonymType synType = synonym->getSynonymType();
    if (synType == CALL || synType == READ || synType == PRINT) {
        if (attribute->getAttrType() == NAME_ATTRTYPE) {
            return getName(synType, std::stoi(value), readFacade);
        }
    }
    return value;
}
