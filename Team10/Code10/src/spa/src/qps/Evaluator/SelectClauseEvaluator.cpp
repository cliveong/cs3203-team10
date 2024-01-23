#include "SelectClauseEvaluator.h"

#include <string>
#include <vector>

#include "qps/Elem.h"
#include "qps/Utils.h"

bool SelectClauseEvaluator::evaluate(const std::shared_ptr<SelectClause>& selectClause, const std::shared_ptr<ResultsTable>& resultsTable, const std::shared_ptr<ReadFacade>& readFacade) {
    std::vector<std::shared_ptr<Elem>> elementList = selectClause->getElements();
    for (auto& element : elementList) {
        std::shared_ptr<Synonym> synonym = Utils::getSynonym(element);
        std::string synonymName = synonym->getSynonymName();
        if (resultsTable->hasResult(synonymName)) {
            continue;
        }
        std::vector<std::string> results = Utils::getEntitiesList(synonym->getSynonymType(), readFacade);
        bool addSuccess = resultsTable->addSingleResult(synonymName, results);
        if (!addSuccess) {
            return false;
        }
    }
    return true;
}
