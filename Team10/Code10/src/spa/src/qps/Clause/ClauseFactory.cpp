#include "ClauseFactory.h"

std::shared_ptr<Clause> ClauseFactory::createClause(const std::string &clauseType, std::shared_ptr<Argument> arg1,
                                                    std::shared_ptr<Argument> arg2, bool notOpr) {
    return std::shared_ptr<Clause>({});
}
