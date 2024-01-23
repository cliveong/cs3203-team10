#pragma once

#include "qps/Clause/Clause.h"

class AffectsClause : public Clause {
   public:
    AffectsClause(std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2, std::string name, bool notOpr);
};
