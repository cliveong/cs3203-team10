#include "WithClauseFactory.h"

#include "WithClause.h"

std::shared_ptr<Clause> WithClauseFactory::createClause(const std::string& clauseType, std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2, bool notOpr) {
    switch (map[clauseType]) {
        case With:
            return std::make_shared<WithClause>(arg1, arg2, clauseType, notOpr);
        default:
            return std::make_shared<Clause>();
    }
}
