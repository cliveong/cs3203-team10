#include "ParentClause.h"

ParentClause::ParentClause(std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2, std::string name, bool notOpr) : Clause(arg1, arg2, name, notOpr) {
}
