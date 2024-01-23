#pragma once

#include "qps/Clause/Clause.h"

class WithClause : public Clause {
   public:
    WithClause(std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2, std::string name, bool notOpr);
};
