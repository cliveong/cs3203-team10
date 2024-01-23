#pragma once

#include <map>

#include "ClauseFactory.h"
#include "qps/Synonym.h"

class PatternClauseFactory : public ClauseFactory {
    enum patternType {
        Assign,
        If,
        While,
        Pattern
    };

   private:
    std::map<std::string, patternType> map = {
        {"AssignPattern", Assign},
        {"IfPattern", If},
        {"WhilePattern", While},
        {"Pattern", Pattern}};

   public:
    std::shared_ptr<Clause> createClause(const std::string& clauseType,
                                         std::shared_ptr<Synonym> synonym,
                                         std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2, bool notOpr);
};