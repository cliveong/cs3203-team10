#pragma once

#include <map>

#include "ClauseFactory.h"
#include "qps/Synonym.h"

class WithClauseFactory : public ClauseFactory {
    enum clauseType {
        With
    };

   private:
    std::map<std::string, clauseType> map = {
        {"WithClause", With}};

   public:
    std::shared_ptr<Clause> createClause(const std::string& clauseType, std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2, bool notOpr);
};
