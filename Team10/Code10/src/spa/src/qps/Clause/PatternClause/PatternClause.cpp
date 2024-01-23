#include "PatternClause.h"

PatternClause::PatternClause(std::shared_ptr<Synonym> synonym, std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2, std::string name, bool notOpr) : Clause(arg1, arg2, name, notOpr) {
    this->synonym = synonym;
}

std::shared_ptr<Synonym> PatternClause::getSynonym() {
    return synonym;
}

bool PatternClause::operator==(const Clause& clause) {
    auto* assignClause = dynamic_cast<const PatternClause*>(&clause);
    if (assignClause == nullptr) {
        return false;
    } else {
        return Clause::operator==(*assignClause) && *(this->synonym) == *((*assignClause).synonym);
    }
}
