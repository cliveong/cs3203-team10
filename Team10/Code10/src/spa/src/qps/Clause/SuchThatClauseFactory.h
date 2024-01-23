#pragma once

#include <map>

#include "Clause.h"
#include "ClauseFactory.h"
class SuchThatClauseFactory : public ClauseFactory {
    enum ClauseType {
        Follows,
        FollowsT,
        Parent,
        ParentT,
        Uses,
        Modifies,
        Calls,
        CallsT,
        Next,
        NextT,
        Affects
    };

   private:
    std::map<std::string, ClauseType> map = {
        {"Follows", Follows},
        {"Follows*", FollowsT},
        {"Parent", Parent},
        {"Parent*", ParentT},
        {"Uses", Uses},
        {"Modifies", Modifies},
        {"Calls", Calls},
        {"Calls*", CallsT},
        {"Next", Next},
        {"Next*", NextT},
        {"Affects", Affects}};

   public:
    std::shared_ptr<Clause> createClause(
        const std::string& clauseType, std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2, bool notOpr);
};