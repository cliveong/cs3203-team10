#include "ClauseEvaluatorFactory.h"

#include <string>

#include "qps/Evaluator/PatternClauseEvaluator/AssignPatternClauseEvaluator.h"
#include "qps/Evaluator/PatternClauseEvaluator/IfPatternClauseEvaluator.h"
#include "qps/Evaluator/PatternClauseEvaluator/WhilePatternClauseEvaluator.h"
#include "qps/Evaluator/SuchThatClauseEvaluator/AffectsClauseEvaluator.h"
#include "qps/Evaluator/SuchThatClauseEvaluator/CallsClauseEvaluator.h"
#include "qps/Evaluator/SuchThatClauseEvaluator/CallsTClauseEvaluator.h"
#include "qps/Evaluator/SuchThatClauseEvaluator/FollowsClauseEvaluator.h"
#include "qps/Evaluator/SuchThatClauseEvaluator/FollowsTClauseEvaluator.h"
#include "qps/Evaluator/SuchThatClauseEvaluator/ModifiesClauseEvaluator.h"
#include "qps/Evaluator/SuchThatClauseEvaluator/NextClauseEvaluator.h"
#include "qps/Evaluator/SuchThatClauseEvaluator/NextTClauseEvaluator.h"
#include "qps/Evaluator/SuchThatClauseEvaluator/ParentClauseEvaluator.h"
#include "qps/Evaluator/SuchThatClauseEvaluator/ParentTClauseEvaluator.h"
#include "qps/Evaluator/SuchThatClauseEvaluator/UsesClauseEvaluator.h"
#include "qps/Evaluator/WithClauseEvaluator.h"

std::shared_ptr<ClauseEvaluator> ClauseEvaluatorFactory::getEvaluator(std::shared_ptr<Clause> clause, std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade) {
    std::string clauseName = clause->getName();
    switch (map[clauseName]) {
        case Follows:
            return std::make_shared<FollowsClauseEvaluator>(resultsTable, readFacade, clause);
        case FollowsT:
            return std::make_shared<FollowsTClauseEvaluator>(resultsTable, readFacade, clause);
        case Parent:
            return std::make_shared<ParentClauseEvaluator>(resultsTable, readFacade, clause);
        case ParentT:
            return std::make_shared<ParentTClauseEvaluator>(resultsTable, readFacade, clause);
        case Uses:
            return std::make_shared<UsesClauseEvaluator>(resultsTable, readFacade, clause);
        case Modifies:
            return std::make_shared<ModifiesClauseEvaluator>(resultsTable, readFacade, clause);
        case Calls:
            return std::make_shared<CallsClauseEvaluator>(resultsTable, readFacade, clause);
        case CallsT:
            return std::make_shared<CallsTClauseEvaluator>(resultsTable, readFacade, clause);
        case AssignPattern:
            return std::make_shared<AssignPatternClauseEvaluator>(resultsTable, readFacade, clause);
        case IfPattern:
            return std::make_shared<IfPatternClauseEvaluator>(resultsTable, readFacade, clause);
        case WhilePattern:
            return std::make_shared<WhilePatternClauseEvaluator>(resultsTable, readFacade, clause);
        case With:
            return std::make_shared<WithClauseEvaluator>(resultsTable, readFacade, clause);
        case Next:
            return std::make_shared<NextClauseEvaluator>(resultsTable, readFacade, clause);
        case NextT:
            return std::make_shared<NextTClauseEvaluator>(resultsTable, readFacade, clause);
        case Affects:
            return std::make_shared<AffectsClauseEvaluator>(resultsTable, readFacade, clause);
        default:
            return std::make_shared<ClauseEvaluator>(resultsTable, readFacade, clause);
    }
}