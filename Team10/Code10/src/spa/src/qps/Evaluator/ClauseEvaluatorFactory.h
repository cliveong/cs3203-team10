#pragma once

#include <map>
#include <memory>

#include "ClauseEvaluator.h"
#include "qps/Clause/Clause.h"

class ClauseEvaluatorFactory {
    enum Clauses {
        Follows,
        FollowsT,
        Parent,
        ParentT,
        Uses,
        Modifies,
        Calls,
        CallsT,
        AssignPattern,
        IfPattern,
        WhilePattern,
        With,
        Next,
        NextT,
        Affects
    };

   private:
    std::map<std::string, Clauses> map = {
        {"Follows", Follows},
        {"Follows*", FollowsT},
        {"Parent", Parent},
        {"Parent*", ParentT},
        {"Uses", Uses},
        {"Modifies", Modifies},
        {"Calls", Calls},
        {"Calls*", CallsT},
        {"AssignPattern", AssignPattern},
        {"IfPattern", IfPattern},
        {"WhilePattern", WhilePattern},
        {"WithClause", With},
        {"Next", Next},
        {"Next*", NextT},
        {"Affects", Affects}};

   public:
    std::shared_ptr<ClauseEvaluator> getEvaluator(std::shared_ptr<Clause> clause, std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade);
};