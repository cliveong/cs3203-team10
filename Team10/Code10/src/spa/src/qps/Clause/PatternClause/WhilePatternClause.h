#pragma once

#include "PatternClause.h"
#include "qps/Synonym.h"

class WhilePatternClause : public PatternClause {
   public:
    WhilePatternClause(std::shared_ptr<Synonym> synonym, std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2, std::string name, bool notOpr);
};
