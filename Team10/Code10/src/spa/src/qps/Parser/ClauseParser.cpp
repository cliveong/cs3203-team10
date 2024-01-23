#include "ClauseParser.h"

#include <utility>

#include "qps/AttrRef.h"
#include "qps/Clause/PatternClauseFactory.h"
#include "qps/Clause/SuchThatClauseFactory.h"
#include "qps/Clause/WithClause.h"
#include "qps/Clause/WithClauseFactory.h"

ClauseParser::ClauseParser(std::shared_ptr<SyntaxValidator> syntaxValidator) : syntaxValidator(std::move(syntaxValidator)) {
    selectClauseParser = std::make_shared<SelectClauseParser>(this->syntaxValidator);
    suchThatClauseParser = std::make_shared<SuchThatClauseParser>(this->syntaxValidator);
    patternClauseParser = std::make_shared<PatternClauseParser>(this->syntaxValidator);
    withClauseParser = std::make_shared<WithClauseParser>(this->syntaxValidator);
}

void ClauseParser::parseAllClauses(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    // Select Clause Parsing
    itr = ++((*selectClauseParser).parse(itr));
    addSelectClause();

    std::string nextTokenStr = (*itr)->getTokenString();
    while (nextTokenStr == "such" || nextTokenStr == "pattern" || nextTokenStr == "with") {
        if (nextTokenStr == "such") {
            itr = parseConstraintClause(itr, suchThatClauseParser);
        } else if (nextTokenStr == "pattern") {
            itr = parseConstraintClause(itr, patternClauseParser);
        } else if (nextTokenStr == "with") {  // with
            itr = parseConstraintClause(itr, withClauseParser);
        }

        nextTokenStr = (*(++itr))->getTokenString();
    }

    // Parsing ended but tokens have not reached the end
    syntaxValidator->validateEndOfQuery(itr);
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator ClauseParser::parseConstraintClause(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr, const std::shared_ptr<ConstraintClauseParser>& parser) {
    if ((*(itr))->getTokenType() != END_OF_QUERY) {
        itr = (*parser).parseClause(itr);

        auto* suchThatParser = dynamic_cast<SuchThatClauseParser*>(&(*parser));
        auto* patternParser = dynamic_cast<PatternClauseParser*>(&(*parser));
        auto* withParser = dynamic_cast<WithClauseParser*>(&(*parser));
        if (suchThatParser != nullptr) {
            std::vector<clauseInfo> clauseInfo = (*suchThatParser).getClauseInfo();
            addSuchThatClauses(clauseInfo);
        } else if (patternParser != nullptr) {
            std::vector<patternClauseInfo> clauseInfo = (*patternParser).getClauseInfo();
            addPatternClauses(clauseInfo);
        } else if (withParser != nullptr) {
            std::vector<clauseInfo> clauseInfo = (*withParser).getClauseInfo();
            addWithClauses(clauseInfo);
        }
        return itr;

    } else {
        return --itr;
    }
}

void ClauseParser::addSelectClause() {
    std::vector<std::shared_ptr<Elem>> elems;
    for (const auto& elem : selectClauseParser->getAllElemInfo()) {
        auto synonym = getSynonymInList(elem.synonymName);
        if (!elem.attrName.empty()) {
            auto attribute = std::make_shared<Attribute>(elem.attrName);
            auto attrRef = std::make_shared<AttrRef>(synonym, attribute);
            elems.emplace_back(attrRef);
        } else {
            elems.emplace_back(synonym);
        }
    }

    if (selectClauseParser->isReturnBool()) {
        auto firstElem = elems[0];
        auto* synonym = dynamic_cast<Synonym*>(&(*firstElem));
        auto* attrRef = dynamic_cast<AttrRef*>(&(*firstElem));
        std::shared_ptr<Synonym> selectedSynonym;

        if (synonym != nullptr) {
            selectedSynonym = std::make_shared<Synonym>((*synonym));
        } else {
            selectedSynonym = (*attrRef).getSynonym();
        }

        if (selectedSynonym->getSynonymType() != UNDECLARED) {
            // synonym BOOLEAN is declared
            this->selectClause = std::make_shared<SelectClause>(elems, false);
        } else {
            // BOOLEAN return
            elems = {};
            this->selectClause = std::make_shared<SelectClause>(elems, true);
        }
    } else {
        this->selectClause = std::make_shared<SelectClause>(elems, selectClauseParser->isReturnBool());
    }
}

void ClauseParser::addSuchThatClauses(const std::vector<clauseInfo>& allClauseInfo) {
    std::shared_ptr<Clause> clause;
    SuchThatClauseFactory suchThatFactory;
    for (const auto& clauseInfo : allClauseInfo) {
        processArgument(clauseInfo.arguments);
        clause = suchThatFactory.createClause(clauseInfo.clauseType, clauseInfo.arguments[0], clauseInfo.arguments[1], clauseInfo.isNot);
        constraintClauses.emplace_back(clause);
    }
}

void ClauseParser::addPatternClauses(const std::vector<patternClauseInfo>& allClauseInfo) {
    std::shared_ptr<Clause> clause;
    PatternClauseFactory patternFactory;
    for (const auto& patternClauseInfo : allClauseInfo) {
        auto clauseInfo = patternClauseInfo.clauseInfo;
        processArgument(clauseInfo.arguments);
        clause = patternFactory.createClause(clauseInfo.clauseType, getSynonymInList(patternClauseInfo.synonymName),
                                             clauseInfo.arguments[0], clauseInfo.arguments[1], clauseInfo.isNot);
        constraintClauses.emplace_back(clause);
    }
}

void ClauseParser::addWithClauses(const std::vector<clauseInfo>& allClauseInfo) {
    std::shared_ptr<Clause> clause;
    WithClauseFactory withClauseFactory;
    for (const auto& clauseInfo : allClauseInfo) {
        processArgument(clauseInfo.arguments);
        clause = withClauseFactory.createClause("WithClause", clauseInfo.arguments[0], clauseInfo.arguments[1], clauseInfo.isNot);
        constraintClauses.emplace_back(clause);
    }
}

std::shared_ptr<Synonym> ClauseParser::getSynonymInList(const std::string& synonymName) {
    for (const auto& synonym : synonyms) {
        if ((*synonym).getSynonymName() == synonymName) {
            return synonym;
        }
    }
    return std::make_shared<Synonym>(UNDECLARED, synonymName);
}

std::shared_ptr<SelectClause> ClauseParser::getSelectClause() {
    return this->selectClause;
}

std::vector<std::shared_ptr<Clause>> ClauseParser::getConstraintClauses() {
    return this->constraintClauses;
}

void ClauseParser::setSynonyms(const std::vector<std::shared_ptr<Synonym>>& synonyms) {
    this->synonyms = synonyms;
}

void ClauseParser::processArgument(const std::vector<std::shared_ptr<Argument>>& arguments) {
    for (const auto& argument : arguments) {
        convertSynonymInArgument(argument);
    }
}

void ClauseParser::convertSynonymInArgument(const std::shared_ptr<Argument>& argument) {
    if (argument->getType() == SYNONYM) {
        argument->setSynonym(getSynonymInList(argument->getValue()));
        argument->setValue("");

    } else if (argument->getType() == STRING_ARG) {
        std::string currentValue = argument->getValue();
        argument->setValue(currentValue.substr(1, currentValue.length() - 2));
    }
}
