#pragma once

#include "qps/Clause/Clause.h"
#include "qps/Synonym.h"

class PatternClause : public Clause {
   private:
    std::shared_ptr<Synonym> synonym;

   public:
    PatternClause(std::shared_ptr<Synonym> synonym, std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2, std::string name, bool notOpr);
    std::shared_ptr<Synonym> getSynonym();
    bool operator==(const Clause& clause) override;
};