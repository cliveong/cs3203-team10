#pragma once
#include <functional>

#include "qps/Evaluator/ClauseEvaluator.h"

class ProcedureProcedureClauseEvaluator : public ClauseEvaluator {
   private:
    bool evaluateProcedureProcedure();
    bool evaluateProcedureString();
    bool evaluateProcedureWildcard();
    bool evaluateStringProcedure();
    bool evaluateStringString();
    bool evaluateStringWildcard();
    bool evaluateWildcardProcedure();
    bool evaluateWildcardString();
    bool evaluateWildcardWildcard();

   protected:
    std::function<std::vector<std::string>(std::string)> getProcedureFirstArg;
    std::function<std::vector<std::string>(std::string)> getProcedureSecondArg;
    std::function<bool(void)> getAllProcedures;

   public:
    ProcedureProcedureClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
    bool evaluateClause() override;
};
