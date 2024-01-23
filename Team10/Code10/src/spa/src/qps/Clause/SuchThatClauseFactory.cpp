#include "SuchThatClauseFactory.h"

#include "Clause.h"
#include "qps/Clause/SuchThatClause/AffectsClause.h"
#include "qps/Clause/SuchThatClause/CallsClause.h"
#include "qps/Clause/SuchThatClause/CallsTClause.h"
#include "qps/Clause/SuchThatClause/FollowsClause.h"
#include "qps/Clause/SuchThatClause/FollowsTClause.h"
#include "qps/Clause/SuchThatClause/ModifiesClause.h"
#include "qps/Clause/SuchThatClause/NextClause.h"
#include "qps/Clause/SuchThatClause/NextTClause.h"
#include "qps/Clause/SuchThatClause/ParentClause.h"
#include "qps/Clause/SuchThatClause/ParentTClause.h"
#include "qps/Clause/SuchThatClause/UsesClause.h"

std::shared_ptr<Clause> SuchThatClauseFactory::createClause(
    const std::string& clauseType, std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2, bool notOpr) {
    switch (map[clauseType]) {
        case Follows:
            return std::make_shared<FollowsClause>(arg1, arg2, clauseType, notOpr);
        case FollowsT:
            return std::make_shared<FollowsTClause>(arg1, arg2, clauseType, notOpr);
        case Parent:
            return std::make_shared<ParentClause>(arg1, arg2, clauseType, notOpr);
        case ParentT:
            return std::make_shared<ParentTClause>(arg1, arg2, clauseType, notOpr);
        case Uses:
            return std::make_shared<UsesClause>(arg1, arg2, clauseType, notOpr);
        case Modifies:
            return std::make_shared<ModifiesClause>(arg1, arg2, clauseType, notOpr);
        case Calls:
            return std::make_shared<CallsClause>(arg1, arg2, clauseType, notOpr);
        case CallsT:
            return std::make_shared<CallsTClause>(arg1, arg2, clauseType, notOpr);
        case Next:
            return std::make_shared<NextClause>(arg1, arg2, clauseType, notOpr);
        case NextT:
            return std::make_shared<NextTClause>(arg1, arg2, clauseType, notOpr);
        case Affects:
            return std::make_shared<AffectsClause>(arg1, arg2, clauseType, notOpr);
        default:
            return std::make_shared<Clause>();
    }
}
