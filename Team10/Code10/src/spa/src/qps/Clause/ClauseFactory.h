#pragma once

#include <memory>
#include <string>

#include "Clause.h"

class ClauseFactory {
   public:
    std::shared_ptr<Clause> createClause(const std::string& clauseType, std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2, bool notOpr);
};