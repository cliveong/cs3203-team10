#include "SemanticValidator.h"

#include <map>
#include <string>
#include <unordered_set>

#include "ValidatorErrors/SemanticError.h"
#include "qps/AttrRef.h"

void SemanticValidator::validateDeclarations(const std::vector<std::shared_ptr<Synonym>>& synonyms) {
    std::unordered_set<std::string> synonymsSet;
    for (auto& synonym : synonyms) {
        bool inserted = synonymsSet.insert(synonym->getSynonymName()).second;
        if (!inserted) {
            throw SemanticError();
        }
    }
}

void SemanticValidator::validateSelectClause(const std::shared_ptr<SelectClause>& selectClause) {
    auto elements = selectClause->getElements();

    for (auto element : elements) {
        if (element->isSynonym()) {
            auto* synonym = dynamic_cast<Synonym*>(&(*element));
            validateSynonymDeclared(std::make_shared<Synonym>(*synonym));
        } else {  // attrRef
            auto* attrRef = dynamic_cast<AttrRef*>(&(*element));
            auto synonym = (*attrRef).getSynonym();
            validateSynonymDeclared(synonym);
            validateAttr(std::make_shared<AttrRef>(*attrRef));
        }
    }
}

void SemanticValidator::validateSynonymDeclared(const std::shared_ptr<Synonym>& synonym) {
    if (synonym->getSynonymType() == UNDECLARED) {
        throw SemanticError();
    }
}

void SemanticValidator::validateSynonymInArgDeclared(const std::shared_ptr<Clause>& clause) {
    auto arg1 = clause->getFirstArg();
    if (arg1->isSynonym()) {
        validateSynonymDeclared(arg1->getSynonym());
    }

    auto arg2 = clause->getSecondArg();
    if (arg2->isSynonym()) {
        validateSynonymDeclared(arg2->getSynonym());
    }
}

bool SemanticValidator::isProcedure(const std::shared_ptr<Synonym>& synonym) {
    return synonym->getSynonymType() == PROCEDURE;
}

bool SemanticValidator::isStatement(const std::shared_ptr<Synonym>& synonym) {
    return synonym->isStatement();
}

bool SemanticValidator::isVariable(const std::shared_ptr<Synonym>& synonym) {
    return synonym->getSynonymType() == VARIABLE;
}

bool SemanticValidator::isWildcard(const std::shared_ptr<Argument>& argument) {
    return argument->getType() == WILDCARD;
}

void SemanticValidator::validateStmtStmtClause(const std::shared_ptr<Clause>& clause) {
    auto arg1 = clause->getFirstArg();
    auto arg2 = clause->getSecondArg();

    if (arg1->getType() == SYNONYM && !isStatement(arg1->getSynonym())) {
        throw SemanticError();
    }

    if (arg2->getType() == SYNONYM && !isStatement(arg2->getSynonym())) {
        throw SemanticError();
    }
}

void SemanticValidator::validateProcProcClause(const std::shared_ptr<Clause>& clause) {
    auto arg1 = clause->getFirstArg();
    auto arg2 = clause->getSecondArg();

    if (arg1->getType() == SYNONYM && !isProcedure(arg1->getSynonym())) {
        throw SemanticError();
    }

    if (arg2->getType() == SYNONYM && !isProcedure(arg2->getSynonym())) {
        throw SemanticError();
    }
}

void SemanticValidator::validateUsesModifiesClause(const std::shared_ptr<Clause>& clause) {
    auto arg1 = clause->getFirstArg();
    auto arg2 = clause->getSecondArg();

    if (isWildcard(arg1)) {
        throw SemanticError();
    }

    if (arg1->getType() == SYNONYM && !isStatement(arg1->getSynonym()) && !isProcedure(arg1->getSynonym())) {
        throw SemanticError();
    }

    if (arg2->getType() == SYNONYM && !isVariable(arg2->getSynonym())) {
        throw SemanticError();
    }
}

void SemanticValidator::validatePattern(const std::shared_ptr<PatternClause>& clause) {
    std::shared_ptr<Synonym> assignSynonym = clause->getSynonym();
    auto arg1 = clause->getFirstArg();

    std::map<std::string, SynonymType> map = {
        {"AssignPattern", ASSIGN},
        {"WhilePattern", WHILE},
        {"IfPattern", IF}};

    if (assignSynonym->getSynonymType() != map[clause->getName()]) {
        throw SemanticError();
    }

    if (arg1->getType() == SYNONYM && !isVariable(arg1->getSynonym())) {
        throw SemanticError();
    }
}

void SemanticValidator::validateWithClause(const std::shared_ptr<WithClause>& clause) {
    std::map<ArgType, AttrType> typeMap = {
        {STRING_ARG, NAME_ATTRTYPE},
        {INT_ARG, INTEGER_ATTRTYPE}};

    auto firstArg = clause->getFirstArg();
    auto firstArgType = firstArg->getType();
    auto secondArg = clause->getSecondArg();
    auto secondArgType = secondArg->getType();

    if (firstArgType == SYNONYM && secondArgType == SYNONYM) {
        auto firstWithArg = std::dynamic_pointer_cast<WithArgument>(firstArg);
        auto secondWithArg = std::dynamic_pointer_cast<WithArgument>(secondArg);

        validateAttr(std::dynamic_pointer_cast<WithArgument>(firstWithArg));
        validateAttr(std::dynamic_pointer_cast<WithArgument>(secondWithArg));

        if (firstWithArg->getAttribute()->getAttrType() != secondWithArg->getAttribute()->getAttrType()) {
            throw SemanticError();
        }

    } else if (firstArgType == SYNONYM) {  // second arg not synonym.attrName
        auto firstWithArg = std::dynamic_pointer_cast<WithArgument>(firstArg);
        validateAttr(std::dynamic_pointer_cast<WithArgument>(firstWithArg));

        auto firstArgAttrType = firstWithArg->getAttribute()->getAttrType();
        if (typeMap[secondArgType] != firstArgAttrType) {
            throw SemanticError();
        }
    } else if (secondArgType == SYNONYM) {  // first arg not synonym.attrName
        auto secondWithArg = std::dynamic_pointer_cast<WithArgument>(secondArg);
        validateAttr(std::dynamic_pointer_cast<WithArgument>(secondWithArg));

        auto secondArgAttrType = secondWithArg->getAttribute()->getAttrType();
        if (typeMap[firstArgType] != secondArgAttrType) {
            throw SemanticError();
        }
    } else {  // both not synonym.attrName
        if (firstArgType != secondArgType) {
            throw SemanticError();
        }
    }
}

void SemanticValidator::validateAttr(const std::shared_ptr<AttrRef>& attrRef) {
    auto synonymType = attrRef->getSynonym()->getSynonymType();
    auto attrType = attrRef->getAttribute()->getAttrName();

    std::map<enum SynonymType, std::unordered_set<enum AttrName>> map{
        {PROCEDURE, {PROCNAME_ATTR}},
        {CALL, {PROCNAME_ATTR, STMT_ATTR}},
        {VARIABLE, {VARNAME_ATTR}},
        {READ, {VARNAME_ATTR, STMT_ATTR}},
        {PRINT, {VARNAME_ATTR, STMT_ATTR}},
        {CONSTANT, {VALUE_ATTR}},
        {STMT, {STMT_ATTR}},
        {WHILE, {STMT_ATTR}},
        {IF, {STMT_ATTR}},
        {ASSIGN, {STMT_ATTR}}};

    auto allowedAttrList = map[synonymType];
    // Attribute not in allowed list
    if (allowedAttrList.find(attrType) == allowedAttrList.end()) {
        throw SemanticError();
    }
}

void SemanticValidator::validateAttr(const std::shared_ptr<WithArgument>& withArg) {
    auto synonymType = withArg->getSynonym()->getSynonymType();
    auto attrType = withArg->getAttribute()->getAttrName();

    std::map<enum SynonymType, std::unordered_set<enum AttrName>> map{
        {PROCEDURE, {PROCNAME_ATTR}},
        {CALL, {PROCNAME_ATTR, STMT_ATTR}},
        {VARIABLE, {VARNAME_ATTR}},
        {READ, {VARNAME_ATTR, STMT_ATTR}},
        {PRINT, {VARNAME_ATTR, STMT_ATTR}},
        {CONSTANT, {VALUE_ATTR}},
        {STMT, {STMT_ATTR}},
        {WHILE, {STMT_ATTR}},
        {IF, {STMT_ATTR}},
        {ASSIGN, {STMT_ATTR}}};

    auto allowedAttrList = map[synonymType];
    // Attribute not in allowed list
    if (allowedAttrList.find(attrType) == allowedAttrList.end()) {
        throw SemanticError();
    }
}

void SemanticValidator::validateConstraintClause(const std::shared_ptr<Clause>& clause) {
    validateSynonymInArgDeclared(clause);

    std::string clauseName = clause->getName();
    if (clauseName == "Follows" || clauseName == "Follows*" || clauseName == "Parent" || clauseName == "Parent*" ||
        clauseName == "Next" || clauseName == "Next*" || clauseName == "Affects") {
        validateStmtStmtClause(clause);
    } else if (clauseName == "Uses" || clauseName == "Modifies") {
        validateUsesModifiesClause(clause);
    } else if (clauseName == "Calls" || clauseName == "Calls*") {
        validateProcProcClause(clause);
    } else if (clauseName == "AssignPattern" || clauseName == "WhilePattern" || clauseName == "IfPattern") {
        validatePattern(std::dynamic_pointer_cast<PatternClause>(clause));
    } else if (clauseName == "UnknownPattern") {
        throw SemanticError();
    } else if (clauseName == "WithClause") {
        validateWithClause(std::dynamic_pointer_cast<WithClause>(clause));
    }
}

void SemanticValidator::validateSemantic(const std::shared_ptr<Query>& query) {
    std::shared_ptr<SelectClause> selectClause = query->getSelectClause();
    std::vector<std::shared_ptr<Clause>> constraintClauses = query->getConstraintClauses();
    std::vector<std::shared_ptr<Synonym>> synonyms = query->getSynonyms();

    // Validate synonyms declared only once
    validateDeclarations(synonyms);

    // Validate Select clause
    validateSelectClause(selectClause);

    // Validate constraint clauses (such that/pattern)
    for (auto& clause : constraintClauses) {
        validateConstraintClause(clause);
    }
}
