#include "Clause.h"

Clause::Clause(std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2, std::string name, bool notOpr) : arg1(arg1), arg2(arg2), name(name), notOpr(notOpr) {}

void Clause::setArguments(std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2) {
    this->arg1 = arg1;
    this->arg2 = arg2;
}

std::shared_ptr<Argument> Clause::getFirstArg() {
    return this->arg1;
}

std::shared_ptr<Argument> Clause::getSecondArg() {
    return this->arg2;
}

std::string Clause::getName() {
    return name;
}

bool Clause::isNot() {
    return notOpr;
}

bool Clause::operator==(const Clause& clause) {
    return (*arg1) == (*(clause.arg1)) && (*arg2) == (*(clause.arg2)) && name == clause.name && notOpr == clause.notOpr;
}
