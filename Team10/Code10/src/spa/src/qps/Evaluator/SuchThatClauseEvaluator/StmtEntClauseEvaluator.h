#pragma once

#include <functional>

#include "qps/Evaluator/ClauseEvaluator.h"

class StmtEntClauseEvaluator : public ClauseEvaluator {
   private:
    bool evaluateStmtVariable();
    bool evaluateStmtString();
    bool evaluateStmtWildcard();
    bool evaluateIntVariable();
    bool evaluateIntString();
    bool evaluateIntWildcard();
    bool evaluateStringVariable();
    bool evaluateStringString();
    bool evaluateStringWildcard();
    bool evaluateProcedureVariable();
    bool evaluateProcedureString();
    bool evaluateProcedureWildcard();

   protected:
    std::function<std::vector<std::string>(int)> getStmtVariables;
    std::function<bool(int, std::string)> checkStmtRelation;
    std::function<std::vector<std::string>(std::string)> getProcVariables;
    std::function<bool(std::string, std::string)> checkProcRelation;

   public:
    StmtEntClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
    bool evaluateClause() override;
};
