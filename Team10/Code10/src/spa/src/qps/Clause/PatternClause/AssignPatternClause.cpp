#include "AssignPatternClause.h"

AssignPatternClause::AssignPatternClause(std::shared_ptr<Synonym> synonym, std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2, std::string name, bool notOpr) : PatternClause(synonym, arg1, arg2, name, notOpr) {}
