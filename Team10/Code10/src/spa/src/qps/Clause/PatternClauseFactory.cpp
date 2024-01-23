#include "PatternClauseFactory.h"

#include "qps/Clause/PatternClause/AssignPatternClause.h"
#include "qps/Clause/PatternClause/IfPatternClause.h"
#include "qps/Clause/PatternClause/WhilePatternClause.h"

std::shared_ptr<Clause> PatternClauseFactory::createClause(const std::string& clauseType, std::shared_ptr<Synonym> synonym, std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2, bool notOpr) {
    switch (map[clauseType]) {
        case Assign:
            return std::make_shared<AssignPatternClause>(synonym, arg1, arg2, clauseType, notOpr);
            break;
        case If:
            return std::make_shared<IfPatternClause>(synonym, arg1, arg2, clauseType, notOpr);
            break;
        case While:
            return std::make_shared<WhilePatternClause>(synonym, arg1, arg2, clauseType, notOpr);
            break;
        case Pattern:
            if (synonym->getSynonymType() == ASSIGN) {
                return std::make_shared<AssignPatternClause>(synonym, arg1, arg2, "AssignPattern", notOpr);
            } else if ((synonym->getSynonymType() == WHILE)) {
                return std::make_shared<WhilePatternClause>(synonym, arg1, arg2, "WhilePattern", notOpr);
            } else {
                return std::make_shared<PatternClause>(synonym, arg1, arg2, "UnknownPattern", notOpr);
            }
        default:
            return std::make_shared<Clause>();
    }
}